<?php
// add default editors to hook
add_filter('qw_edit_themes', 'qw_edit_themes_default', 0);

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
function qw_edit_themes_default($themes){
  $themes['tabs'] = array(
    'title' => 'Tabs',
    'init_callback' => 'qw_edit_theme_tabs_init',
  );
  $themes['views'] = array(
    'title' => 'Drupal Views',
    'init_callback' => 'qw_edit_theme_views_init',
  );
  return $themes;
}

/*
 * Edit Theme: Jquery UI Tabs
 */
function qw_edit_theme_tabs_init(){
  add_action('admin_head', 'qw_edit_theme_tabs_css');
  if($_GET['page'] == 'query-wrangler'){
    add_action('admin_enqueue_scripts', 'qw_edit_theme_tabs_js');
  }
}
// css
function qw_edit_theme_tabs_css(){
  print '<link rel="stylesheet" type="text/css" href="'.QW_PLUGIN_URL.'/admin/editors/tabs/tabs.css" />';
}
// js
function qw_edit_theme_tabs_js(){
  wp_enqueue_script('qw-admin-theme-tabs',
                  plugins_url('/admin/editors/tabs/tabs.js', dirname(__FILE__)),
                  array('jquery-ui-core',
                        'jquery-ui-sortable',
                        'jquery-ui-tabs',
                        'jquery-ui-accordion',
                        'jquery-ui-dialog'),
                  QW_VERSION);
}

/*
 * Edit Theme: Drupal Views clone
 */
function qw_edit_theme_views_init(){
  add_action('admin_head', 'qw_edit_theme_views_css');
  if($_GET['page'] == 'query-wrangler'){
    add_action('admin_enqueue_scripts', 'qw_edit_theme_views_js');
  }
}
// css
function qw_edit_theme_views_css(){
  print '<link rel="stylesheet" type="text/css" href="'.QW_PLUGIN_URL.'/admin/editors/views/views.css" />';
}
// js
function qw_edit_theme_views_js(){
  // my js script
  wp_enqueue_script('qw-edit-theme-views',
                  plugins_url('/admin/editors/views/views.js', dirname(__FILE__) ),
                  array('jquery-ui-core',
                        'jquery-ui-sortable',
                        'jquery-ui-tabs',
                        'jquery-ui-accordion',
                        'jquery-ui-dialog'),
                  QW_VERSION);
}
