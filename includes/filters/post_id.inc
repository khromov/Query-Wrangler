<?php
// hook into qw_all_filters()
add_filter('qw_filters', 'qw_filter_post_id');

/*
 * Add filter to qw_filters
 */
function qw_filter_post_id($filters){
  $filters['post_id'] = array(
    'title' => 'Post IDs',
    'description' => 'Provide a list of post_ids to show or not show.',
    'form_callback' => 'qw_filter_post_id_form',
    'query_args_callback' => 'qw_filter_post_id_args',
    // exposed
    'exposed_form' => 'qw_filter_post_id_exposed_form',
    'exposed_process' => 'qw_filter_post_id_exposed_process',
    //'exposed_settings_form_callback' => 'qw_filter_post_id_exposed_settings_form',
  );
  return $filters;
}

/*
 * Convert values into query args
 */
function qw_filter_post_id_args(&$args, $filter){
  $pids = explode(",", $filter['values']['post_ids']);
  array_walk($pids, 'qw_trim');
  $args[$filter['values']['compare']] = $pids;
}

/*
 * Filter form
 */
function qw_filter_post_id_form($filter)
{ ?>
    <input type='text' name="<?php print $filter['form_prefix']; ?>[post_ids]" value='<?php print $filter['values']['post_ids']; ?>' />
    <p class="description">How to treat these post IDs.</p>

    <p>
      <select name="<?php print $filter['form_prefix']; ?>[compare]">
        <option value="post__in" <?php if($filter['values']['compare'] == "post__in") { print 'selected="selected"';} ?>>Only these posts</option>
        <option value="post__not_in" <?php if($filter['values']['compare'] == "post__not_in") { print 'selected="selected"';} ?>>Not these posts</option>
      </select>
    </p>
  <?php
}

/*
 * Process submitted exposed form values
 */
function qw_filter_post_id_exposed_process(&$args, $filter, $values){
  // default values if submitted is empty
  if(isset($filter['values']['exposed_default_values'])){
    // value can be '0', so check if null
    if (is_null($values)){
      $values = $filter['values']['post_ids'];
    }
  }

  // make into array
  $values = explode(",", $values);

  // check allowed values
  if (isset($filter['values']['exposed_limit_values'])){
    // loop through and check allowed values
    $allowed = explode(",", $filter['values']['post_ids']);
    foreach($values as $k => $value){
      if (!in_array($value, $allowed)){
        unset($values[$k]);
      }
    }
    $args[$filter['compare']] = $values;
  }
  else {
    $args[$filter['compare']] = $values;
  }
}

/*
 * Exposed form
 */
function qw_filter_post_id_exposed_form($filter, $values)
{
  // default values
  if (isset($filter['values']['exposed_default_values'])){
    if (is_null($values)){
      $values = $filter['values']['post_ids'];
    }
  }
  ?>
  <input type="text"
         name="<?php print $filter['exposed_key']; ?>"
         value="<?php print $values ?>" />
  <?php
}
//function qw_filter_post_id_exposed_settings_form($filter){}
