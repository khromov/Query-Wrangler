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
    'form_prefix'   => '[display][field_settings][fields]',
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

function qw_handler_field_data($options) {
  return $options['display']['field_settings']['fields'];
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