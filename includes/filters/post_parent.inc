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
    'query_display_types' => array('page','widget'),
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