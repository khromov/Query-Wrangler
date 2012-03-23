<?php
// hook into qw_all_filters()
add_filter('qw_filters', 'qw_filter_post_parent');

/*
 * Add filter to qw_filters
 */
function qw_filter_post_parent($filters){
  $filters['post_parent'] = array(
    'title' => 'Post Parent',
    'description' => 'Use only with post type "Page" to show results with the chosen parent ID.',
    'form_callback' => 'qw_filter_post_parent_form',
    // exposed
    'exposed_form' => 'qw_filter_post_parent_exposed_form',
    'exposed_process' => 'qw_filter_post_parent_exposed_process',
    //'exposed_settings_form_callback' => 'qw_filter_post_parent_exposed_settings_form',
  );
  return $filters;
}

/*
 * Filter form
 */
function qw_filter_post_parent_form($filter)
{ ?>
  <input class="qw-field-value"
         type="text"
         name="<?php print $filter['form_prefix']; ?>[post_parent]"
         value="<?php print $filter['values']['post_parent']; ?>" />
  <?php
}
/*
 * Process submitted exposed form values
 */
function qw_filter_post_parent_exposed_process(&$args, $filter, $values){
  // default values if submitted is empty
  qw_filter_post_parent_exposed_default_values($filter, $values);

  // check allowed values
  if (isset($filter['values']['exposed_limit_values'])){
    if ($values == $filter['values']['post_parent']){
      $args['post_parent'] = $values;
    }
  }
  else {
    $args['post_parent'] = $values;
  }
}

/*
 * Exposed form
 */
function qw_filter_post_parent_exposed_form($filter, $values)
{
  // default values
  qw_filter_post_parent_exposed_default_values($filter, $values);
  ?>
  <input type="text"
         name="<?php print $filter['exposed_key']; ?>"
         value="<?php print $values; ?>" />
  <?php
}
//function qw_filter_post_parent_exposed_settings_form($filter){}
/*
 * Simple helper funtion to handle default values
 */
function qw_filter_post_parent_exposed_default_values($filter, &$values){
  if (isset($filter['values']['exposed_default_values'])){
    if (is_null($values)){
      $values = $filter['values']['post_parent'];
    }
  }
}
