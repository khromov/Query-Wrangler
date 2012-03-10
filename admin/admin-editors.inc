<?php
// tw hook
add_filter('tw_templates', 'qw_editors_templates');

/*
 * Template Wrangler templates
 *
 * @param $templates array Passed from the filter hook from WP
 *
 * @return array All template arrays filtered so far by Wordpress' filter hook
 */
function qw_editors_templates($templates){
  // edit query template
  $templates['query_edit'] = array(
    'files' => 'admin/editors/[theme]/[theme]-editor.php',
    'default_path' => QW_PLUGIN_DIR,
    'arguments' => array(
      'theme' => get_option('qw_edit_theme'),
    ),
  );
  return $templates;
}


/*
 * Initialize editor themes
 */
function qw_editors_init(){

  // include editor's incs
  $dirs = scandir(QW_PLUGIN_DIR.'/admin/editors');
  foreach($dirs as $dir){
    if ($dir != '.' || $dir != '..'){
      $editor_inc = QW_PLUGIN_DIR.'/admin/editors/'.$dir.'/'.$dir.'.inc';
      if (file_exists($editor_inc)){
        include_once $editor_inc;
      }
    }
  }

  $themes = qw_all_editors();
  $current = get_option('qw_edit_theme');
  if(isset($themes[$current])){
    $theme = $themes[$current];
  } else {
    $theme = $themes[QW_DEFAULT_THEME];
  }

  if(function_exists($theme['init_callback'])){
    $theme['init_callback']();
  }
}