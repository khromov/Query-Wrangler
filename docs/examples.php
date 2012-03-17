<?php

/* ===== Example Field ============================== */

// hook
add_filter('qw_fields', 'qw_field_example');

/*
 * My new field definition
 */
function qw_field_example($fields)
{
  // new field
  $fields['example_field'] = array(

    // title displayed to query-wrangler user
    'title' => 'Example: Field',

    // description on the field form
    'description' => 'Just a useful description of this field',

    // optional) callback for outputting a field, must return the results
    'output_callback' => 'qw_field_example_output',

    // (optional) where or not to pass $post and $field into the output_callback
    //    useful for custom functions
    'output_arguments' => true,

    // (optional) callback function for field forms
    'form_callback' => 'qw_field_example_form_callback',
  );
  return $fields;
}

/*
 * Example output callback with output_arguments = true
 *
 * @param $post  The WP $post object
 * @param $field This field's settings and values. Values stored in $field['values']
 */
function qw_field_example_output($post, $field){
  // adjust output according to my custom field settings
  if ($field['values']['my_setting'] == 'title'){
    $output = $post->post_title;
  }
  else if ($field['values']['my_setting'] == 'status'){
    $output = $post->post_status;
  }
  return $output;
}

/*
 * Provide a settings form for this field
 *
 * Output is expected of all forms, because they are executed within a buffer
 *
 * @param $field  - this field's settings and values
                    Values stored in $field['values']
 */
function qw_field_example_form_callback($field)
{
  // retrieve the value from the field for retaining settings values
  $value = $field['values']['my_setting'];
  ?>
  <select name="<?php print $field['form_prefix']; ?>[my_setting]">
    <option value="title" <?php if ($value == 'title'){ print 'selected="selected"';} ?>>Show Post Title</option>
    <option value="status" <?php if ($value == 'status'){ print 'selected="selected"';} ?>>Show Post Status</option>
  </select>
  <?php
}


/* ===== Example Sort Option ========================== */

// add default field styles to the filter
add_filter('qw_sort_options', 'qw_example_sort_options');

/*
 * Sorts are generally very generic.
 *
 * Most WP_Query sort arguments have a very simple values and structures.
 * Since in QW it is a handler, it has alot of other available options.
 */
function qw_example_sort_options($sort_options)
{
  $sort_options['example_sort'] = array(
    // the title displayed to the query-wrangler user
    'title' => 'Example: Sort Option',

    // some help text for the user about how this sort option works
    'description' => 'A description of how this sort option works.',

    // (optional) This is the value of the WP_Query argument orderby_key
    // defaults to: the hook_key
      // $args[$sort['orderby_key']] = $sort['type'];
    'type' => 'wp_query_argument_key',

    // (optional) the WP_Query argument key equivalent to WP_Query's 'orderby'
    // defaults to: 'orderby'
    'orderby_key' => 'my_orderby_key',

    // (optional) the WP_Query argument key equivalent to WP_Query's 'order'
    // defaults to: 'order'
    'order_key' => 'my_order_key',

    // (optional) order options provided in a select menu
    // defaults to:  below values
    'order_options' => array(
      'ASC' => 'Ascending',
      'DESC' => 'Descending',
    ),

    // (optional) a custom callback function for placing form values into a WP_Query as arguments
    // defaults to:
      //  $args[$sort['orderby_key']] = $sort['type'];
      //  $args[$sort['order_key']] = $sort['order_value'];
    'query_args_callback' => 'qw_sort_example_query_args',

    // (optional) a custom callback for sort options forms
    // if callback and template both aren't set,
    //   defaults to:  'qw_sorts_default_form_callback'
    'form_callback' => 'my_sort_option_form_callback',

    // (optional) a template wrangler form template
    'form_template' => 'my_tw_form_template',
  );
  return $sort_options;
}

/*
 * Doing this is so simple that qw will do it for you if you don't provide a callback.
 * But this is what it looks like if you were to do it yourself
 *
 *  @param  &$args - The WP_Query arguments we are building
 *  @param  $filter - This filter's settings and values
                      Values stored in $filter['values']
 */
function qw_sort_example_query_args(&$args, $sort){
  $args[$sort['orderby_key']] = $sort['type'];
  $args[$sort['order_key']] = $sort['order_value'];
}

/* ===== Example Filter ============================= */

// hook into qw_all_filters()
add_filter('qw_filters', 'qw_filter_example');

/*
 * Add filter to qw_filters
 */
function qw_filter_example($filters)
{
  // new filter
  $filters['example_filter'] = array(
    // title shown when selecting a filter
    'title' => 'Example: Filter',

    // help text for the user
    'description' => 'Description of this filter',

    // (optional) the query argument key
    // if doesn't exist, defaults to the hook_key.
    // if confused what this is for, don't use this
    'type' => 'filter_type',

    // ! This or a form_template must be used
    // * (optional) callback for form
    'form_callback' => 'qw_filter_example_form_callback',

    // * (optional) template wrangler theme function or template file
    'form_template' => 'my_tw_template_hook',

    // (optional) generate_args callback
    // determines how form data becomes WP_Query arguments
    // defaults to form key as WP_Query argument key
    'query_args_callback' => 'qw_filter_example_query_args',

    // (optional) the form exposed to a user above the query
    'exposed_form' => 'qw_filter_example_exposed_form',

    // (optional) process the exposed filter's values into the query args
    'exposed_process' => 'qw_filter_example_exposed_process',

    // (optional) a form for gather settings for the exposed filter
    'exposed_settings_form' => 'qw_filter_example_exposed_settings_form',
  );
  return $filters;
}

/*
 * Example of custom filter form.
 *
 * @param $filter - This filter's settings and saved values
 *                  Values stored in $filter['values']
 */
function qw_filter_example_form_callback($filter)
{ ?>
  <label class="qw-label">My filter setting</label>
  <input type='text'
         name="<?php print $filter['form_prefix']; ?>[my_setting]"
         value='<?php print $filter['values']['my_setting']; ?>' />
  <?php
}

/*
 * Convert the filter settings into a WP_Query argument
 *
 *  @param  &$args - The WP_Query arguments being built
 *  @param  $filter - This filter's settings and saved values
 *                    Values stored in $filter['values']
 */
function qw_filter_example_query_args(&$args, $filter){
  $args['some_wp_query_argument'] = $filter['values']['my_setting'];
}

/*
 * Filter exposed form
 *
 *  @param  $filter - This filter's settings and values
 *                    Values stored in $filter['values']
 *  @param  $values - Submitted values for this exposed_key
 */
function qw_filter_example_exposed_form($filter, $values)
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

/*
 * Example, processing exposed submitted values into the WP_Query
 *
 *  @param  &$args - The WP_Query args being built
 *  @param  $filter - This filter's settings and values
 *                    Values stored in $filter['values']
 *  @param  $values - The submitted value for this exposed filter
 *                   Equivalent to $d = qw_exposed_submitted_data(); $d[$filter['exposed_key']];
 */
function qw_filter_example_exposed_process(&$args, $filter, $values){
  // default values if submitted is empty
  if(isset($filter['values']['exposed_default_values'])){
    if (is_null($values)){
      $values = $filter['values']['my_setting'];
    }
  }

  // check allowed values
  if (isset($filter['values']['exposed_limit_values'])){
    if ($values == $filter['values']['my_setting']){
      $args['some_wp_query_argument'] = $values;
    }
  }
  else {
    $args['some_wp_query_argument'] = $values;
  }
}

/*
 * Example additional settings for an exposed form
 *
 *  @param  $filter - This filter's settings and values
 *                    Values stored in $filter['values']
 */
function qw_filter_example_exposed_settings_form($filter)
{
  // use the default provided single/multiple exposed values
  // saves values to [exposed_settings][type]
  print qw_exposed_setting_type($filter);
}
/* ===== Example Contextual Filter ============================= */
