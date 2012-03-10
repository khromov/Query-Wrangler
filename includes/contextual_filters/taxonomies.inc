<?php
// hook into qw_all_contextual_filters()
add_filter('qw_contextual_filters', 'qw_contextual_filter_taxonomies');

/*
 * Add filter to qw_contextual_filters
 */
function qw_contextual_filter_taxonomies($contextual_filters){
  $ts = get_taxonomies(array('_builtin' => false), 'objects');
  if(count($ts) > 0){
    foreach ($ts as $t){
      $contextual_filters['taxonomy_'.$t->name] = array(
        'title' => 'Taxonomy: '. $t->label,
        'description' => 'Select which taxonomies to accept contexts from, and how to treat those taxonomies.',
        'form_callback' => 'qw_contextual_filter_taxonomies_form',
        'allowed_query_types' => array('page','widget'),
        'query_args_callback' => 'qw_contextual_filter_taxonomies_args',
        // allow this contextual_filter to override pages
        'can_override' => true,
        // additional update callback for saving a query
        'query_update_callback' => 'qw_contextual_filter_taxonomies_query_update',
        // modify the override options
        'query_override_options_callback' => 'qw_contextual_filter_taxonomies_override_options',
        // return the appropriate override_id
        'query_override_id_callback' => 'qw_contextual_filter_taxonomies_override_id',
        // which query op should this context act on?
        'query_op' => 'tax',
        'query_var' => $t->query_var,
      );
    }
  }
  return $contextual_filters;
}

/*
 * Convert contextual values into query args
 */
function qw_contextual_filter_taxonomies_args(&$args, $contextual_filter){
  if (!is_array($args['tax_query'])){
    $args['tax_query'] = array();
  }
  $terms = get_query_var($contextual_filter['query_var']);
  krumo($terms);
  $args['tax_query'][] = array(
    'taxonomy' => $contextual_filter['query_var'],
    'field' => 'id',
    'terms' => array_keys($contextual_filter['values']['terms']),
    'operator' => $filter['values']['operator'],
  );
}

/*
 * Return override_options for this contextual filter
 *
 * Basically, add a new filter to the query, and provide it values
 */
function qw_contextual_filter_taxonomies_override_options($override_id){
  if ($term = qw_get_term($override_id)){
    global $wpdb;
    // get taxonomy data
    $sql = "SELECT taxonomy FROM ".$wpdb->prefix."term_taxonomy WHERE term_id = ".$term->term_id;
    $tax = $wpdb->get_row($sql);

    $options_override = array();
    $options_override['args']['filters']['context_taxonomy'] = array (
      'type' => 'taxonomy_'.$tax->taxonomy,
      'name' => 'taxonomy_'.$tax->taxonomy,
      'hook_key' => 'taxonomy_'.$tax->taxonomy,
      'terms' =>  array (
        $term->term_id => $term->name,
      ),
      'operator' => 'IN',
    );
    return $options_override;
  }
}
/*
 * Return the appropriate override_id by looking for query_vars
 */
function qw_contextual_filter_taxonomies_override_id($contextual_filter){
  $term = get_query_var($contextual_filter['query_var']);

  // make sure we only have 1
  if(is_array($term)){
     $term = array_pop($term);
  }

  $term = qw_get_term($term, 'slug');
  if(isset($term->term_id)){
    return $term->term_id;
  }
}
/*
 *
 */
function qw_contextual_filter_taxonomies_query_update($contextual_filter){
  // save items to db table if not exists.
  $new_values = array_keys($contextual_filter['values']['terms']);
  qw_update_override_values($contextual_filter, $new_values);
}

/*
 * Filter form
 */
function qw_contextual_filter_taxonomies_form($contextual_filter)
{
  $terms = get_terms($contextual_filter['query_var'], array('hide_empty' => false));
  ?>
    <div class="qw-checkboxes">
      <?php
        // List all taxonomies as checkboxes
        foreach($terms as $term)
        {
          $term_checked = (isset($contextual_filter['values']['terms'][$term->term_id])) ? 'checked="checked"' : '';
          ?>
          <label class="qw-query-checkbox">
            <input type="checkbox"
                   name="<?php print $contextual_filter['form_prefix']; ?>[terms][<?php print $term->term_id; ?>]"
                   value="<?php print $term->name; ?>"
                   <?php print $term_checked; ?> />
            <?php print $term->name; ?>
          </label>
          <?php
        }
      ?>
    </div>
  <?php
}