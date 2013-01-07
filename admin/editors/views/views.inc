<?php
/*
 * Edit Theme: Drupal Views clone
 */

// add default editors to hook
add_filter('qw_editors', 'qw_editors_views');
// tw hook
add_filter('tw_templates', 'qw_editors_views_templates');

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
function qw_editors_views($themes){
  $themes['views'] = array(
    'title' => 'Drupal Views',
    'init_callback' => 'qw_editors_views_init',
  );
  return $themes;
}


function qw_editors_views_templates($templates){
  $templates['editor_views_add_handler'] = array(
    'files' => 'admin/editors/views/views-add-handler.php',
    'default_path' => QW_PLUGIN_DIR,
    'arguments' => array(
      'handler' => array(),
    ),
  );

  return $templates;
}

// ini callback
function qw_editors_views_init(){
  add_action('admin_head', 'qw_editors_views_css');
  if($_GET['page'] == 'query-wrangler'){
    add_action('admin_enqueue_scripts', 'qw_editors_views_js');
  }
}

// css
function qw_editors_views_css(){
  print '<link rel="stylesheet" type="text/css" href="'.QW_PLUGIN_URL.'/admin/editors/views/views.css" />';
}
// js
function qw_editors_views_js(){
  // my js script
  wp_enqueue_script('qw-edit-theme-views',
                  plugins_url('views.js', __FILE__ ),
                  array('qw-admin-js',
                        'jquery-ui-core',
                        'jquery-ui-sortable',
                        'jquery-ui-tabs',
                        'jquery-ui-accordion',
                        'jquery-ui-dialog',
                  ),
                  QW_VERSION, true);
}
