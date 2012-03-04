<?php
// hook into qw_all_filters()
add_filter('qw_filters', 'qw_filter_taxonomies');

/*
 * Add filter to qw_filters
 */
function qw_filter_taxonomies($filters){
  $ts = get_taxonomies(array('_builtin' => false), 'objects');
  if(count($ts) > 0){
    foreach ($ts as $t){
      $filters['taxonomy_'.$t->name] = array(
        'title' => 'Taxonomy: '.$t->label,
        'query_var' => $t->query_var,
        'description' => 'Filter by Custom taxonomy: '.$t->label,
        'form_callback' => 'qw_filter_taxonomies_form',
        'query_args_callback' => 'qw_filter_taxonomies_args',
      );
    }
  }
  return $filters;
}


/*
 * Convert values into query args
 */
function qw_filter_taxonomies_args(&$args, $filter){
  if (!is_array($args['tax_query'])){
    $args['tax_query'] = array();
  }

  $args['tax_query'][] = array(
    'taxonomy' => $filter['query_var'],
    'field' => 'id',
    'terms' => array_keys($filter['values']['terms']),
    'operator' => $filter['values']['operator'],
  );
}

/*
 * Filter form
 */
function qw_filter_taxonomies_form($filter)
{
  $terms = get_terms($filter['query_var'], array('hide_empty' => false));
  ?>
  <div class="qw-checkboxes">
    <?php
      // List all categories as checkboxes
      foreach($terms as $term)
      {
        $term_checked = (isset($filter['values']['terms'][$term->term_id])) ? 'checked="checked"' : '';
        ?>
        <label class="qw-query-checkbox">
          <input type="checkbox"
                 name="<?php print $filter['form_prefix']; ?>[terms][<?php print $term->term_id; ?>]"
                 value="<?php print $term->name; ?>"
                 <?php print $term_checked; ?> />
          <?php print $term->name; ?>
        </label>
        <?php
      }
    ?>
  </div>
  <p><strong>Taxonomy Options</strong> - show posts that:</p>
  <select name="<?php print $filter['form_prefix']; ?>[operator]" class="qw-field-value">
    <option value="IN" <?php if($filter['values']['operator'] == "IN") {print 'selected="selected"';} ?>>
      Have one of the terms select
    </option>
    <option value="NOT IN" <?php if($filter['values']['operator'] == "NOT IN") {print 'selected="selected"';} ?>>
      Not in the terms selected
    </option>
    <option value="AND" <?php if($filter['values']['operator'] == "AND") {print 'selected="selected"';} ?>>
      Have all of the terms selected
    </option>
  </select>
  <?php
}