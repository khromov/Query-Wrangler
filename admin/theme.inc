<?php
/*
 * Handles all theme related functions for admin pages
 */

// tw hook
add_filter('tw_templates', 'qw_admin_templates');

/*
 * Template Wrangler templates
 *
 * @param $templates array Passed from the filter hook from WP
 *
 * @return array All template arrays filtered so far by Wordpress' filter hook
 */
function qw_admin_templates($templates){
  // preview query
  $templates['query_preview'] = array(
    'files' => 'admin/page-preview.php',
    'default_path' => QW_PLUGIN_DIR,
    'arguments' => array('options' => NULL),
  );
  
  // edit query template
  $templates['query_edit'] = array(
    'files' => 'admin/editors/[theme]/[theme]-editor.php',
    'default_path' => QW_PLUGIN_DIR,
    'arguments' => array(
      'theme' => get_option('qw_edit_theme'),
    ),
  );

  // create query template
  $templates['query_create'] = array(
    'files' => 'admin/page-create.php',
    'default_path' => QW_PLUGIN_DIR,
  );

  // import query template
  $templates['query_import'] = array(
    'files' => 'admin/page-import.php',
    'default_path' => QW_PLUGIN_DIR,
  );

  // export query template
  $templates['query_export'] = array(
    'files' => 'admin/page-export.php',
    'default_path' => QW_PLUGIN_DIR,
    'arguments' => array(
      'query_id' => 0,
    ),
  );

  // settings query template
  $templates['query_settings'] = array(
    'files' => 'admin/page-settings.php',
    'default_path' => QW_PLUGIN_DIR,
  );

  // useful for plugin admin pages
  $templates['admin_wrapper'] = array(
    'files' => 'admin/wrapper-page.php',
    'default_path' => QW_PLUGIN_DIR,
    'arguments' => array(
        'title' => 'Admin Page',
        'content' => 'content goes here.',
    ),
  );

  // filters edit form
  $templates['query_filter'] = array(
    'files' => 'admin/wrapper-filter.php',
    'default_path' => QW_PLUGIN_DIR,
  );

  // filters
  $filters = qw_all_filters();
  foreach($filters as $type => $filter){
    // form template
    if(isset($filter['form_template'])){
      $templates[$filter['form_template']] = array(
        'arguments' => array(
          'filter' => array()
        )
      );
    }
    // exposed settings form templates
    if(isset($filter['exposed_settings_form_template'])){
      $templates[$filter['exposed_settings_form_template']] = array(
        'arguments' => array(
          'filter' => array()
        )
      );
    }
  }

  // sorts edit form
  $templates['query_sort'] = array(
    'files' => 'admin/wrapper-sort.php',
    'default_path' => QW_PLUGIN_DIR,
  );

  // sorts
  $sorts = qw_all_sort_options();
  foreach($sorts as $type => $sort){
    if(isset($sort['form_template'])){
      $templates[$sort['form_template']] = array(
        'arguments' => array(
          'sort' => array()
        )
      );
    }
    // exposed settings form templates
    if(isset($sort['exposed_settings_form_template'])){
      $templates[$sort['exposed_settings_form_template']] = array(
        'arguments' => array(
          'sort' => array()
        )
      );
    }
  }

  // field template
  $templates['query_field'] = array(
    'files' => 'admin/wrapper-field.php',
    'default_path' => QW_PLUGIN_DIR,
  );

  // fields
  $fields = qw_all_fields();
  foreach($fields as $type => $field){
    if(isset($field['form_template'])){
      $templates[$field['form_template']] = array(
        'arguments' => array(
          'field' => array()
        )
      );
    }
  }

  return $templates;
}