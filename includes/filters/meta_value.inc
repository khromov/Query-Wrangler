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
    'query_display_types' => array('page','widget','override'),
    // exposed
    'exposed_process' => 'qw_filter_meta_value_exposed_process',
    //'exposed_settings_form' => 'qw_filter_meta_value_exposed_settings_form',
    'exposed_form' => 'qw_filter_meta_value_exposed_form',
  );
  return $filters;
}

/*
 * Convert values into query args
 */
function qw_filter_meta_value_args(&$args, $filter){
  $args['meta_value'] = stripslashes($filter['meta_value']);
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
function qw_filter_meta_value_exposed_process(&$args, $filter){
  $values = $_GET[$filter['exposed_key']];
  if(!empty($values)){
    $args[$filter['compare']] = explode(",", $values);
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
