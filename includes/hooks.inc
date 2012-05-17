<?php
/*
 * All Handlers
 *
 * Handlers are groups of items that can be added and removed from a query
 * eg: filters, sorts, fields
 */
function qw_all_handlers(){
  $handlers = apply_filters('qw_handlers', array());
  foreach ($handlers as $hook_key => $handler) {
    $handlers[$hook_key]['hook_key'] = $hook_key;
    // adjust form prefix
    $handlers[$hook_key]['form_prefix'] = 'qw-query-options'.$handlers[$hook_key]['form_prefix'];
    $handlers[$hook_key]['all_items'] = $handler['all_callback']();
  }
  return $handlers;
}
/*
 * All contexts selectable on a context filter
 */
function qw_all_contexts(){
  $contexts = apply_filters('qw_contexts',array());
  return $contexts;
}
/*
 * Editor themes
 */
function qw_all_editors(){
  $themes = apply_filters('qw_editors', array());
  return $themes;
}
/*
 * Basic Settings
 */
function qw_all_basic_settings(){
  $basics = apply_filters('qw_basics', array());

  foreach($basics as $hook_key => $basic){
    if ($basic['type'] == 'page') {
      $basics[$hook_key]['form_prefix'] = 'qw-query-options[display]['.$basic['type'].']';
    }
    else {
      $basics[$hook_key]['form_prefix'] = 'qw-query-options['.$basic['type'].']';
    }
    $basics[$hook_key]['hook_key'] = $hook_key;
  }
  return $basics;
}
/*
 * Fields Hook
 */
function qw_all_fields(){
  $fields = apply_filters('qw_fields', array());
  foreach($fields as $hook_key => $field){
    if(!isset($field['type'])){
      $fields[$hook_key]['type'] = $hook_key;
    }
    // maintain the hook's key
    $fields[$hook_key]['hook_key'] = $hook_key;
  }

  // sort them by title
  $titles = array();
  foreach ($fields as $key => $field) {
    $titles[$key] = $field['title'];
  }
  array_multisort($titles, SORT_ASC, $fields);

  return $fields;
}

/*
 * contextual_filters Hook
 */
function qw_all_contextual_filters(){
  $contextual_filters = apply_filters('qw_contextual_filters', array());

  foreach($contextual_filters as $hook_key => $contextual_filter){
    // set contextual_filter's type as a value if not provided by contextual_filter
    if(!isset($contextual_filter['type'])){
      $contextual_filters[$hook_key]['type'] = $hook_key;
    }
    // maintain the hook's key
    $contextual_filters[$hook_key]['hook_key'] = $hook_key;
  }

  // sort them by title
  $titles = array();
  foreach ($contextual_filters as $key => $contextual_filter) {
    $titles[$key] = $contextual_filter['title'];
  }
  array_multisort($titles, SORT_ASC, $contextual_filters);

  return $contextual_filters;
}

/*
 * filters Hook
 */
function qw_all_filters(){
  $filters = apply_filters('qw_filters', array());

  foreach($filters as $hook_key => $filter){
    // set filter's type as a value if not provided by filter
    if(!isset($filter['type'])){
      $filters[$hook_key]['type'] = $hook_key;
    }

    // maintain the hook's key
    $filters[$hook_key]['hook_key'] = $hook_key;
  }

  // sort them by title
  $titles = array();
  foreach ($filters as $key => $filter) {
    $titles[$key] = $filter['title'];
  }
  array_multisort($titles, SORT_ASC, $filters);

  return $filters;
}

/*
 * Sort Options Hook
 */
function qw_all_sort_options(){
  $sort_options = apply_filters('qw_sort_options', array());

  // set some defaults for very simple hooks
  foreach($sort_options as $hook_key => $option){
    if(!isset($option['type'])){
      $sort_options[$hook_key]['type'] = $hook_key;
    }
    if(!isset($option['orderby_key'])){
      $sort_options[$hook_key]['orderby_key'] = 'orderby';
    }
    if(!isset($option['order_key'])){
      $sort_options[$hook_key]['order_key'] = 'order';
    }
    if(!isset($option['order_options'])){
      $sort_options[$hook_key]['order_options'] =array(
        'ASC' => 'Ascending',
        'DESC' => 'Descending',
      );
    }

    // default form_callback
    if(!isset($option['form_callback']) && !isset($option['form_template'])){
      $sort_options[$hook_key]['form_callback'] = 'qw_sorts_default_form_callback';
    }

    // maintain hook's key
    $sort_options[$hook_key]['hook_key'] = $hook_key;
  }

  // sort them by title
  $titles = array();
  foreach ($sort_options as $key => $sort) {
    $titles[$key] = $sort['title'];
  }
  array_multisort($titles, SORT_ASC, $sort_options);

  return $sort_options;
}

/*
 * Post Statuses
 */
function qw_all_post_statuses(){
  $post_statuses = apply_filters('qw_post_statuses', array());
  return $post_statuses;
}

/*
 * Styles Hook
 */
function qw_all_styles(){
  $styles = apply_filters('qw_styles', array());

  foreach($styles as $hook_key => $style){
    $styles[$hook_key]['hook_key'] = $hook_key;
    $styles[$hook_key]['form_prefix'] = 'qw-query-options[display][style_settings]['.$hook_key.']';
  }
  return $styles;
}

/*
 * Row Styles Hook
 */
function qw_all_row_styles(){
  $row_styles = apply_filters('qw_row_styles', array());
  foreach($row_styles as $hook_key => $row_style){
    $row_styles[$hook_key]['hook_key'] = $hook_key;
    $row_styles[$hook_key]['form_prefix'] = 'qw-query-options[display][row_style_settings]['.$hook_key.']';
  }
  return $row_styles;
}

/*
 * Row Style 'Posts' Styles Hook
 */
function qw_all_row_complete_styles(){
  $row_complete_styles = apply_filters('qw_row_complete_styles', array());

  return $row_complete_styles;
}

/*
 * Row Style 'Fields' Stlyes Hook
 */
function qw_all_row_fields_styles(){
  $row_fields_styles = apply_filters('qw_row_fields_styles', array());
  return $row_fields_styles;
}

/*
 * File Styles Hook
 */
function qw_all_file_styles(){
  $styles = apply_filters('qw_file_styles', array());
  return $styles;
}

/*
 * Post types
 */
function qw_all_post_types(){
  $post_types = apply_filters('qw_post_types', array());

  // Get all verified post types
  $post_types = get_post_types(array('public' => true, '_builtin' => false), 'names', 'and');
  // Add standard types
  $post_types['post'] = 'post';
  $post_types['page'] = 'page';
  // sort types
  ksort($post_types);

  return $post_types;
}

/*
 * Pager types
 */
function qw_all_pager_types(){
  $pagers = apply_filters('qw_pager_types', array());
  return $pagers;
}
/*
 * Wizards... not what I entended.  Will change.
 */
function qw_all_wizards(){
  $wizards = apply_filters('qw_wizards', array());
  return $wizards;
}
/*
 * File Styles Hook
 */
function qw_all_includes(){
  $includes = apply_filters('qw_includes', array());
  return $includes;
}
/*
 * Display types
 */
function qw_all_display_types(){
  $display_types = apply_filters('qw_display_types', array());
  return $display_types;
}