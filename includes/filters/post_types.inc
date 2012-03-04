<?php
// hook into qw_all_filters()
add_filter('qw_filters', 'qw_filter_post_types');

/*
 * Add filter to qw_filters
 */
function qw_filter_post_types($filters){
  $filters['post_types'] = array(
    'title' => 'Post Type',
    'description' => 'Select which post types should be shown.',
    'form_callback' => 'qw_filter_post_types_form',
    'query_args_callback' => 'qw_filter_post_types_args',
  );
  return $filters;
}

/*
 * Convert values into query args
 */
function qw_filter_post_types_args(&$args, $filter){
  $args['post_type'] = $filter['values']['post_types'];
}


/*
 * Filter form
 */
function qw_filter_post_types_form($filter)
{ ?>
  <div class="qw-checkboxes">
    <?php
      $post_types = qw_all_post_types();
      // loop through post types
      foreach ($post_types  as $post_type )
      {
        $post_type_checked = (isset($filter['values']['post_types'][$post_type])) ? 'checked="checked"' : '';
        ?>
        <label class="qw-query-checkbox">
          <input type="checkbox"
                 name="<?php print $filter['form_prefix']; ?>[post_types][<?php print $post_type; ?>]"
                 value="<?php print $post_type; ?>"
                 <?php print $post_type_checked; ?> />
          <?php print ucfirst($post_type); ?>
        </label>
        <?php
      }
    ?>
  </div>
  <?php
}