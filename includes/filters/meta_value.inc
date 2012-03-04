<?php
// hook into qw_all_filters()
add_filter('qw_filters', 'qw_filter_meta_value');

/*
 * Add filter to qw_filters
 */
function qw_filter_meta_value($filters){
  $filters['meta_value'] = array(
    'title' => 'Meta Value',
    'description' => 'Filter for a specific meta_value.',
    'form_callback' => 'qw_filter_meta_value_form',
    'query_args_callback' => 'qw_filter_meta_value_args',
    // exposed
    'exposed_form' => 'qw_filter_meta_value_exposed_form',
    'exposed_process' => 'qw_filter_meta_value_exposed_process',
    //'exposed_settings_form_callback' => 'qw_filter_meta_value_exposed_settings_form',
  );
  return $filters;
}

/*
 * Convert values into query args
 */
function qw_filter_meta_value_args(&$args, $filter){
  $args['meta_value'] = stripslashes($filter['values']['meta_value']);
}

/*
 * Filter form
 */
function qw_filter_meta_value_form($filter)
{ ?>
    <textarea name="<?php print $filter['form_prefix']; ?>[meta_value]"
              class="qw-meta-value"><?php print stripcslashes($filter['values']['meta_value']); ?></textarea>
  <?php
}

/*
 * Process submitted exposed form values
 */
function qw_filter_meta_value_exposed_process(&$args, $filter, $value){
  if(!empty($value)){
    $args['meta_value'] = $value;
  }
}

/*
 * Exposed form
 */
function qw_filter_meta_value_exposed_form($filter)
{ ?>
  <input type="text"
         name="<?php print $filter['exposed_key']; ?>"
         value="<?php print $_GET[$filter['exposed_key']]; ?>" />
  <?php
}
//function qw_filter_meta_value_exposed_settings_form($filter){}
