<?php
add_filter('qw_handlers', 'qw_handlers_default');
add_filter('tw_templates', 'qw_handlers_templates');

/*
 * Default handlers
 */
function qw_handlers_default($handlers){
  $handlers['field'] = array(
    'title' => 'Field',
    'description' => 'Select Fields to add to this query output.',
    'data_callback' => 'qw_handler_field_data',
    'all_callback'  => 'qw_all_fields',
    'form_prefix'   => '[display][fields]',
    'wrapper_template' => 'query_field',
  );
  $handlers['sort'] = array(
    'title' => 'Sort Option',
    'description' => 'Select options for sorting the query results.',
    'data_callback' => 'qw_handler_sort_data',
    'all_callback'  => 'qw_all_sort_options',
    'form_prefix'   => '[args][sorts]',
    'wrapper_template' => 'query_sort',
  );
  $handlers['filter'] = array(
    'title' => 'Filter',
    'description' => 'Select filters to affect the query results.',
    'data_callback' => 'qw_handler_filter_data',
    'all_callback'  => 'qw_all_filters',
    'form_prefix'   => '[args][filters]',
    'wrapper_template' => 'query_filter',
  );
  $handlers['contextual_filter'] = array(
    'title' => 'Contextual Filter',
    'description' => 'Select contextual filters to affect the query results based on the context of where the query is displayed.',
    'data_callback' => 'qw_handler_contextual_filter_data',
    'all_callback'  => 'qw_all_contextual_filters',
    'form_prefix'   => '[args][contextual_filters]',
    'wrapper_template' => 'query_contextual_filter',
  );

  return $handlers;
}

/*
 * Template Wrangler templates
 */
function qw_handlers_templates($templates){

  $handlers = qw_all_handlers();

  foreach($handlers as $type => $handler){
    // wrapper edit form
    $templates[$handler['wrapper_template']] = array(
      'files' => 'admin/wrapper-'.$type.'.php',
      'default_path' => QW_PLUGIN_DIR,
    );

    // all handler items
    $all = $handler['all_callback']();

    // look for templates within all items
    foreach($all as $type => $item){
      // form template
      if(isset($item['form_template'])){
        $templates[$item['form_template']] = array(
          'arguments' => array(
            $type => array()
          )
        );
      }
      // exposed settings form templates
      if(isset($item['exposed_settings_form_template'])){
        $templates[$item['exposed_settings_form_template']] = array(
          'arguments' => array(
            $type => array()
          )
        );
      }
    }
  }
  return $templates;
}

/*
 * Simple functions for getting handler data from the query options
 */
function qw_handler_field_data($options) {
  return $options['display']['fields'];
}
function qw_handler_sort_data($options){
  return $options['args']['sorts'];
}
function qw_handler_filter_data($options){
  return $options['args']['filters'];
}
function qw_handler_contextual_filter_data($options){
  return $options['args']['contextual_filters'];
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

  // Contextual Filter override form
  // see if item has an exposed settings form
  if (function_exists($handler['override_form_callback'])) {
    ob_start();
      $handler['override_form_callback']($handler);
    $handler['override_form'] = ob_get_clean();
  }
  // provide template wrangler support
  else if (isset($handler['override_form_template'])){
    $handler['override_form'] = theme($handler['override_form_template'], array('this' => $handler));
  }
//  krumo($handler['title'].' - '.$handler['hook_key']);
//krumo($handler);
}