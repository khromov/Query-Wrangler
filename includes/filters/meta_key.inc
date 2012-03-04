<?php
// hook into qw_all_filters()
add_filter('qw_filters', 'qw_filter_meta_key');

/*
 * Add filter to qw_filters
 */
function qw_filter_meta_key($filters){
  $filters['meta_key'] = array(
    'title' => 'Meta Key',
    'description' => 'Filter for a specific meta_key.',
    'form_callback' => 'qw_filter_meta_key_form',
    'query_args_callback' => 'qw_filter_meta_key_args',
    // exposed
    'exposed_form' => 'qw_filter_meta_key_exposed_form',
    'exposed_process' => 'qw_filter_meta_key_exposed_process',
    //'exposed_settings_form_callback' => 'qw_filter_meta_key_exposed_settings_form',
  );
  return $filters;
}

/*
 * Filter form
 */
function qw_filter_meta_key_form($filter)
{ ?>
    <input type='text'
           name="<?php print $filter['form_prefix']; ?>[meta_key]"
           value='<?php print $filter['values']['meta_key']; ?>' />
  <?php
}

/*
 * Process submitted exposed form values
 */
function qw_filter_meta_key_exposed_process(&$args, $filter, $value){
  if(!empty($value)){
    $args['meta_key'] = $value;
  }
}

/*
 * Exposed form
 */
function qw_filter_meta_key_exposed_form($filter)
{
  ?>
  <input type="text"
         name="<?php print $filter['exposed_key']; ?>"
         value="<?php print $_GET[$filter['exposed_key']]; ?>" />
  <?php
}
//function qw_filter_exposed_settings_form_meta_key($filter){}
