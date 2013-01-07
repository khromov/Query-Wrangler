<?php
/*
 * Edit Theme: Jquery UI Tabs
 */

// add default editors to hook
add_filter('qw_editors', 'qw_editors_tabs');
// tw hook
add_filter('tw_templates', 'qw_editors_tabs_templates');

/*
 * Default edit themes
 *
 *  template

    $themes['hook_key'] = array(
      // pretty theme name
      'title' => 'Nice Name for Theme',

      // callback for admin_init actions when theme is loaded
      // useful for loading js and css files
      'init_callback' => 'my_init_callback',
    );
 */
function qw_editors_tabs($themes){
  $themes['tabs'] = array(
    'title' => 'Tabs',
    'init_callback' => 'qw_editors_tabs_init',
  );
  return $themes;
}

function qw_editors_tabs_templates($templates){
  $templates['editor_tabs_handler'] = array(
    'files' => 'admin/editors/tabs/tabs-handler.php',
    'default_path' => QW_PLUGIN_DIR,
    'arguments' => array(
      'handler' => array(),
    ),
  );
  $templates['editor_tabs_add_handler'] = array(
    'files' => 'admin/editors/tabs/tabs-add-handler.php',
    'default_path' => QW_PLUGIN_DIR,
    'arguments' => array(
      'handler' => array(),
    ),
  );

  return $templates;
}

// init callback
function qw_editors_tabs_init(){
  add_action('admin_head', 'qw_editors_tabs_css');
  if($_GET['page'] == 'query-wrangler'){
    add_action('admin_enqueue_scripts', 'qw_editors_tabs_js');
  }
}
// css
function qw_editors_tabs_css(){
  print '<link rel="stylesheet" type="text/css" href="'.QW_PLUGIN_URL.'/admin/editors/tabs/tabs.css" />';
}
// js
function qw_editors_tabs_js(){
  wp_enqueue_script('qw-admin-theme-tabs',
                  plugins_url('/tabs/tabs.js', dirname(__FILE__)),
                  array('jquery-ui-core',
                        'jquery-ui-sortable',
                        'jquery-ui-tabs',
                        'jquery-ui-accordion',
                        'jquery-ui-dialog'),
                  QW_VERSION, true);
}
