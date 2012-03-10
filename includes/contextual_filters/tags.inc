<?php
// hook into qw_all_contextual_filters()
add_filter('qw_contextual_filters', 'qw_contextual_filter_tags');

/*
 * Add filter to qw_contextual_filters
 */
function qw_contextual_filter_tags($contextual_filters){
  $contextual_filters['tags'] = array(
    'title' => 'Tags',
    'description' => 'Select which tags to accept contexts from, and how to treat those tags.',
    'form_callback' => 'qw_contextual_filter_tags_form',
    'allowed_query_types' => array('page','widget'),
    'can_override' => true,
    'query_update_callback' => 'qw_contextual_filter_tags_query_update',

    // modify the override options
    'query_override_options_callback' => 'qw_contextual_filter_tags_override_options',

    // return the appropriate override_id
    'query_override_id_callback' => 'qw_contextual_filter_tags_override_id',

    // which query op should this context act on?
    'query_op' => 'tag',
  );
  return $contextual_filters;
}

/*
 * Return override_options for this contextual filter
 */
function qw_contextual_filter_tags_override_options($override_id){

  if ($term = qw_get_term($override_id)){
    $options_override = array();
    // override options
    $options_override['args']['filters']['context_tags']['type'] = 'tags';
    $options_override['args']['filters']['context_tags']['tag_operator'] = 'tag__in';
    $options_override['args']['filters']['context_tags']['tags'] = array($term->term_id => $term->name);
    return $options_override;
  }
}
/*
 * Return the appropriate override_id by looking for query_vars
 */
function qw_contextual_filter_tags_override_id($contextual_filter){
  $term = get_query_var('tag');

  // make sure we only have 1
  if(is_array($term)){
     $term = array_pop($term);
  }

  $term = qw_get_term($term, 'slug');
  if(isset($term->term_id)){
    return $term->term_id;
  }
}

function qw_contextual_filter_tags_query_update($contextual_filter){
  // save items to db table if not exists.
  qw_update_override_values($contextual_filter['query_id'], $contextual_filter['values']['tags']);
}

/*
 * Filter form
 */
function qw_contextual_filter_tags_form($contextual_filter)
{
  $tags = get_tags(array('hide_empty' => false));
  ?>
    <div class="qw-checkboxes">
      <?php
        foreach($tags as $tag)
        {
          $tag_checked = (isset($contextual_filter['values']['tags'][$tag->term_id])) ? 'checked="checked"' : '';
          ?>
          <label class="qw-query-checkbox">
            <input type="checkbox"
                   name="<?php print $contextual_filter['form_prefix']; ?>[tags][<?php print $tag->term_id; ?>]"
                   value="<?php print $tag->name; ?>"
                   <?php print $tag_checked; ?> />
            <?php print $tag->name; ?>
          </label>
          <?php
        }
      ?>
    </div>
  <?php
}