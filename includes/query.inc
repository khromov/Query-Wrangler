<?php
/*
 * Primary function for bulding and displaying a query
 *
 * @param int $query_id Id for the query
 * @param array $options_override an array for changing or adding query data options
 * @param bool $reset_post_data Reset the $wp_query after execution
 * @return string Can return a string of html based on parameter $return
 */
function qw_execute_query($query_id, $options_override = array(), $reset_post_data = true)
{
$_SESSION['qw']['time']['execute']['start'] = microtime(1);
  global $wp_query;
  // backup the old query
  $old_query = $wp_query;

  // get the query options
  $options = qw_generate_query_options($query_id, $options_override);

  // get formatted query arguments
  $args = qw_generate_query_args($options);

// pre_query hook
  do_action_ref_array('qw_pre_query', array(&$args));

  // set the new query
  $wp_query = new WP_Query($args);

// pre_render hook
  do_action_ref_array('qw_pre_render', array(&$options));

$_SESSION['qw']['time']['execute']['end'] = microtime(1);

$_SESSION['qw']['time']['template']['start'] = microtime(1);
  // get the themed content
  $themed = qw_template_query($wp_query, $options);
$_SESSION['qw']['time']['template']['end'] = microtime(1);

  // Reset Post Data
  if($reset_post_data){
    wp_reset_postdata();
    $wp_query = $old_query;
  }

  return $themed;
}
/*
 * Get the Query, and set $options to defaults
 *
 * @param int
 *   $query_id - The unique ID of the query
 * @param array
 *   $options_override - An array of values to override in the retrieved set
 * @param array
 *   $full_override - force the options_override as all options
 *
 * @return array
 *   Query options
 */
function qw_generate_query_options($query_id, $options_override = array(), $full_override = false)
{
  global $wpdb, $wp_query;
  $table_name = $wpdb->prefix."query_wrangler";
  $sql = "SELECT id,name,type,slug,data FROM ".$table_name." WHERE id = ".$query_id;

  $rows = $wpdb->get_results($sql);

  // unserealize the stored data
  $options = qw_unserialize($rows[0]->data);

  // override options
  if ($full_override) {
    // force a full override
    $options = $options_override;
  } else {
    // combine options
    $options =  array_merge_recursive((array)$options, $options_override);
  }

  // build query_details
  $options['meta'] = array();
  $options['meta']['id'] = $rows[0]->id;
  $options['meta']['slug'] = $rows[0]->slug;
  $options['meta']['name'] = $rows[0]->name;
  $options['meta']['type'] = $rows[0]->type;
  $options['meta']['pagination'] = (isset($options['args']['paged'])) ? true : false;
  $options['meta']['header'] = $options['display']['header'];
  $options['meta']['footer'] = $options['display']['footer'];
  $options['meta']['empty'] = $options['display']['empty'];

  return $options;
}

/*
 * Generate the WP query itself
 *
 * @param array $options Query data
 * $param array $options_override can change the options array
 * @return array Query Arguments
 */
function qw_generate_query_args($options = array(), $options_override = array())
{
  // combine options
  $options =  array_merge_recursive((array)$options, $options_override);

  $handlers = qw_preprocess_handlers($options);

  // standard arguments
  $args['posts_per_page'] = ($options['args']['posts_per_page']) ? $options['args']['posts_per_page']: 5;
  $args['offset']         = ($options['args']['offset']) ? $options['args']['offset']: 0;
  $args['paged']          = $options['args']['paged'];
  $args['post_status']    = $options['args']['post_status'];

  $submitted_data = qw_exposed_submitted_data();

  foreach ($handlers as $handler_type => $handler){
    if (is_array($handler['items'])){
      foreach($handler['items'] as $name => $item)
      {
        // Exposed items
        if (isset($item['exposed_form'])){
          if(!empty($item['values']['exposed_key'])){
            // override exposed key
            $item['exposed_key'] = $item['values']['exposed_key'];
          } else {
            // default exposed key
            $item['exposed_key'] = 'exposed_'.$item['values']['name'];
          }
        }

        // Alter the query args
        // look for callback, and run it
        if (isset($item['query_args_callback']) && function_exists($item['query_args_callback'])){
          $item['query_args_callback']($args, $item);
        } else if (isset($item['orderby_key']) && isset($item['order_key'])){
          // else, default to type as WP_Query argument key
          // arguments passed to query
          $args[$item['orderby_key']] = $item['type'];
          $args[$item['order_key']] = $item['values']['order_value'];
        }

        // Process submitted exposed values
        // exposed items
        if(isset($item['exposed_process']) && function_exists($item['exposed_process'])){
          $value = $submitted_data[$item['exposed_key']];
          $item['exposed_process']($args, $item, $value);
        }
      }
    }
  }
  return $args;
}
function qw_exposed_submitted_data(){
  $data = array();
  if (!empty($_GET)){
    $data = $_GET;
  }
  else if (!empty($_POST)){
    $data = $_POST;
  }

  foreach($data as $k => $v){
    if (empty($data[$k])){
      unset($data[$k]);
      unset($_GET[$k]);
      unset($_POST[$k]);
    }
    else {
      $data[$k] = urldecode($v);
    }
  }

  if (count($data) > 0){
    return $data;
  }
  return false;
}
/*
 * Organize an existing filters and give it all the data they needs
 *
 * @param $type
 *   handler-type = 'filter', 'field', 'sort'
 */
function qw_preprocess_handlers($options){
  // build handlers data
  $handlers = qw_all_handlers();
  // Retrieve the handler items from the query array
  foreach ($handlers as $k => $handler){
    if (function_exists($handler['data_callback'])){
      $handlers[$k]['items'] = $handler['data_callback']($options);
    }
  }

  foreach($handlers as $type => $handler){
    // load all our default handlers
    if (is_array($handler['items']))
    {
      $all = $handler['all_items'];

      // generate the form name prefixes
      foreach($handler['items'] as $name => $values)
      {
        // load sort type data
        $hook_key = qw_get_hook_key($all, $values);

        $this_item = $all[$hook_key];

        // move type, hook_key, and weight to top level of array
        $this_item['type'] = $values['type']; unset($values['type']);
        $this_item['weight'] = $values['weight']; unset($values['weight']);
        $this_item['hook_key'] = $values['hook_key']; unset($values['hook_key']);

        // values are own array
        $this_item['values'] = $values;
        $this_item['name'] = $name;
        $this_item['form_prefix'] = $handler['form_prefix'].'['.$name.']';

        qw_handler_make_form($this_item);

        // set new item
        $handlers[$type]['items'][$name] = $this_item;
      }
    }
  }

  // sort sorts according to weight
  if(is_array($handler['items'])){
    uasort($handler['items'], 'qw_cmp');
  }

  return $handlers;
}

/*
 * Look for handler forms and settings forms and execute the callbacks
 */
function qw_handler_make_form(&$handler){
  // this handler's form
  if (function_exists($handler['form_callback'])){
    ob_start();
      $handler['form_callback']($handler);
    $handler['form'] = ob_get_clean();
  }
  // provide template wrangler support
  else if (isset($handler['form_template'])){
    $handler['form'] = theme($handler['form_template'], array('this' => $handler));
  }

  // see if item has an exposed settings form
  if (function_exists($handler['exposed_settings_form_callback'])) {
    ob_start();
      $handler['exposed_settings_form_callback']($handler);
    $handler['exposed_settings_form'] = ob_get_clean();
  }
  // provide template wrangler support
  else if (isset($handler['exposed_settings_form_template'])){
    $handler['exposed_settings_form'] = theme($handler['exposed_settings_form_template'], array('this' => $handler));
  }
//  krumo($handler['title'].' - '.$handler['hook_key']);
//krumo($handler);
}
/*
 * Get all queries of the type widget
 *
 * @return array of query widgets with key as query id
 */
function qw_get_all_widgets()
{
  global $wpdb;
  $table_name = $wpdb->prefix."query_wrangler";
  $sql = "SELECT id,name FROM ".$table_name." WHERE type = 'widget'";
  $rows = $wpdb->get_results($sql);

  if(is_array($rows))
  {
    $widgets = array();
    foreach($rows as $row){
      $widgets[$row->id] = $row->name;
    }
    return $widgets;
  }
}
/*
 * Get all query pages
 *
 * @return array Query pages in WP post format
 */
function qw_get_all_pages()
{
  global $wpdb;
  $table_name = $wpdb->prefix."query_wrangler";
  $sql = "SELECT id,name,path FROM ".$table_name." WHERE type = 'page'";
  $rows = $wpdb->get_results($sql);

  if(is_array($rows)){
    $pages = array();
    $blog_url = get_bloginfo('wpurl');

    $i=0;
    foreach($rows as $row){
      $pages[$i] = new stdClass();
      $pages[$i]->ID = $row->id;
      $pages[$i]->title = $row-name;
      $pages[$i]->post_title = $row->name;
      $pages[$i]->guid = $blog_url.$row->path;
      $pages[$i]->post_type = 'page';
    }

    return $pages;
  }
}

/*
 * Helper function: Get the current page number
 * @param object $wp_query - the query being displayed
 *
 * @return
 *    int - the currentpage number
 */
function qw_get_page_number(&$wp_query){
  // help figure out the current page
  $path_array = explode('/page/', $_SERVER['REQUEST_URI']);

  // look for WP paging first
  if ($wp_query->query_vars['paged']){
    $page = $wp_query->query_vars['paged'];
  }
  // try wordpress method
  else if (get_query_var('paged')){
    $page = get_query_var('paged');
  }
  // paging with slashes
  else if (isset($path_array[1])) {
    $page = explode('/', $path_array[1]);
    $page = $page[0];
  }
  // paging with get variable
  else if (isset($_GET['page'])) {
    $page = $_GET['page'];
  }
  // paging with a different get variable
  else if (isset($_GET['paged'])) {
    $page = $_GET['paged'];
  }
  else {
    $page = 1;
  }
  return $page;
}
/*
 * Get full term data
 *
 * @param $term
 *   - either term id or term slug based on 2nd parameter
 * @param $by
 *   - either 'id' or 'slug': what you want the get term by
 * @param $return_type
 *  - OBJECT, ARRAY_A, ARRAY_N
 *
 * @return
 *  - term, format depending on 3rd parameter
 *  - false if not found
 */
function qw_get_term($term, $by = 'id' , $return_type = OBJECT){
  global $wpdb;
  switch ($by){
    case 'id':
      $where = 't.term_id = '.$term;
      break;

    case 'slug':
      $where = 't.slug = "'.$term.'"';
      break;
  }

  $sql = "SELECT
            t.term_id,t.name,t.slug,t.term_group,tax.taxonomy,tax.description,tax.parent,tax.count
          FROM ".$wpdb->prefix."terms as t
          LEFT JOIN ".$wpdb->prefix."term_taxonomy as tax ON t.term_id = tax.term_id
          WHERE ".$where;
  $t = $wpdb->get_row($sql, $return_type);

  if ($t->term_id){
    // http://web.archiveorange.com/archive/v/XZYvyS8D7kDM3sQgrvJF
    $t->link = get_term_link((int)$t->term_id, $t->taxonomy);
    // return term if found
    return $t;
  }
  else {
    return false;
  }
}

/*
 * Trim each item in an array w/ array_walk
 *   eg: array_walk($fruit, 'qw_trim');
 */
function qw_trim(&$value){
  $value = trim($value);
}
/*
 * Serialize wrapper functions for future changes.
 */
function qw_serialize($array){
  return serialize($array);
}
/*
 * Custom: Fix unserialize problem with quotation marks
 */
function qw_unserialize($serial_str) {
  $serial_str= preg_replace('!s:(\d+):"(.*?)";!se', "'s:'.strlen('$2').':\"$2\";'", $serial_str );
  $serial_str= str_replace("\r", "", $serial_str);
  //$serial_str= str_replace("\n", "", $serial_str);
  $array = array();
  $array = unserialize($serial_str);
  if(is_array($array)){
    // stripslashes twice for science
    $array = array_map('stripslashes_deep', $array);
    $array = array_map('stripslashes_deep', $array);
    return $array;
  }
}
/*
 * Support function for legacy, pre hook_keys discovery
 */
function qw_get_hook_key($all, $single)
{
  // see if hook key is set
  if (!empty($single['hook_key'])){
    $hook_key = $single['hook_key'];
  }
  // look for type as key
  else if (!empty($single['type']))
  {
    foreach ($all as $key => $item)
    {
      if($single['type'] == $all[$key]['type']){
        $hook_key = $all[$key]['hook_key'];
        break;
      }
      else if($single['type'] == $key){
        $hook_key = $key;
        break;
      }
    }
  }
  return $hook_key;
}

/*
 * usort callback. I likely stole this from somewhere.. like php.net
 */
function qw_cmp($a,$b) {
  if ($a['weight'] == $b['weight']) return 0;
  return ($a['weight'] < $b['weight'])? -1 : 1;
}
