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
  $templates['preview_wrapper'] = array(
    'files' => 'admin/preview-wrapper.php',
    'default_path' => QW_PLUGIN_DIR,
  );
  $templates['preview_ajax'] = array(
    'files' => 'admin/preview-ajax.inc',
    'default_path' => QW_PLUGIN_DIR,
    'arguments' => array('options' => NULL),
  );

  // create query template
  $templates['query_create'] = array(
    'files' => 'admin/page-create.php',
    'default_path' => QW_PLUGIN_DIR,
    'arguments' => array(
      'wizards' => array(),
    )
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

  // import query template
  $templates['query_docs'] = array(
    'files' => 'docs/index.php',
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

  return $templates;
}