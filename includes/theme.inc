<?php
/*
 * Handles all theme related functions for output
 */

// tw hook
add_filter('tw_templates', 'qw_templates');

/*
 * Template Wrangler templates
 *
 * @param $templates array Passed from the filter hook from WP
 *
 * @return array All template arrays filtered so far by Wordpress' filter hook
 */
function qw_templates($templates){

  // display queries style wrapper
  $templates['query_display_wrapper'] = array(
    'files' => array(
      'query-wrapper-[slug].php',
      'query-wrapper.php',
      'templates/query-wrapper.php',
    ),
    'default_path' => QW_PLUGIN_DIR,
    'arguments' => array(
      'slug' => '',
    )
  );
  // full and field styles
  $templates['query_display_rows'] = array(
    'files' => array(
      '[template]-[slug].php',
      '[template].php',
      'templates/[template].php',
    ),
    'default_path' => QW_PLUGIN_DIR,
    'arguments' => array(
      'template' => 'query-unformatted',
      'slug' => 'not-found',
      'style' => 'unformatted',
      'rows' => array(),
    )
  );

  return $templates;
}
/*
 * Preprocess query_display_rows to allow field styles to define their own default path
 */
function theme_query_display_rows_preprocess($template)
{
  // make sure we know what style to use
  if(isset($template['arguments']['style']))
  {
    // get the specific style
    $all_styles = qw_all_styles();

    // set this template's default path to the style's default path
    if(isset($all_styles[$template['arguments']['style']])){
      $style = $all_styles[$template['arguments']['style']];
      $template['default_path'] = $style['default_path'];
    }
  }
  return $template;
}
/*
 * Template the entire query
 *
 * @param object
 *   $wp_query Wordpress query object
 * @param array
 *   $options the query options
 *
 * @return string HTML for themed/templated query
 */
function qw_template_query(&$wp_query, $options)
{
  $results_count = count($wp_query->posts);
  $content = '';

  // start building theme arguments
  $wrapper_args = array(
    'slug' => $options['meta']['slug'],
  );

  // look for empty results
  if ($results_count > 0)
  {
    $all_styles = qw_all_styles();

    $style = $all_styles[$options['display']['style']];

     // setup row template arguments
     $template_args = array(
       'template' => 'query-'.$style['hook_key'],
       'slug' => $options['meta']['slug'],
       'style' => $style['hook_key'],
       'style_settings' => $options['display']['style_settings'][$style['hook_key']],
       'options' => $options,
      );

    // the content of the widget is the result of the query
    if($options['display']['row_style'] == "posts") {
      $template_args['rows'] = qw_make_posts_rows($wp_query, $options);
    }
      // setup row template arguments
    else if ($options['display']['row_style'] == "fields"){
      $template_args['rows'] = qw_make_fields_rows($wp_query, $options);
    }

    // template the query rows
    $wrapper_args['content'] = theme('query_display_rows', $template_args);
  }
  // empty results
  else {
    // no pagination
    $options['meta']['pagination'] = false;
    // show empty text
    $wrapper_args['content'] = '<div class="query-empty">'.$options['meta']['empty'].'</div>';
  }

  $wrapper_classes = array();
  $wrapper_classes[] = 'query';
  $wrapper_classes[] = 'query-'.$options['meta']['slug'].'-wrapper';
  $wrapper_classes[] = $options['display']['wrapper-classes'];
  $wrapper_args['wrapper_classes'] = implode(" ", $wrapper_classes);

  // header
  if($options['meta']['header'] != '') {
    $wrapper_args['header'] = $options['meta']['header'];
  }
  // footer
  if($options['meta']['footer'] != '') {
    $wrapper_args['footer'] = $options['meta']['footer'];
  }

  // pagination
  if($options['meta']['pagination'] && isset($options['display']['page']['pager']['active'])){
    $pager_classes = array();
    $pager_classes[] = 'query-pager';
    $pager_classes[] = 'pager-'.$options['display']['page']['pager']['type'];
    $wrapper_args['pager_classes'] = implode(" ", $pager_classes);
    // pager
    $wrapper_args['pager'] = qw_make_pager($options['display']['page']['pager'], $wp_query);
  }

  $exposed = qw_generate_exposed_handlers($options);
  if(count($exposed) > 0){
    $wrapper_args['exposed'] =  $exposed;
  }

  // template with wrapper
  return theme('query_display_wrapper', $wrapper_args);
}
/*
 * Create rows for Posts row_style query
 */
function qw_make_posts_rows(&$wp_query, $options){
  $rows = array();
  $i = 0;
  while($wp_query->have_posts())
  {
    $wp_query->the_post();
    $template_args = array(
      'template' => 'query-'.$options['display']['post_settings']['size'],
      'slug' => $options['meta']['slug'],
      'style' => $options['display']['post_settings']['size'],
    );
    $rows[$i]['row_classes'] = qw_row_classes($i);
    $field_classes = array('query-post-wrapper');
    $rows[$i]['fields'][$i]['classes'] = implode(" ",$field_classes);
    $rows[$i]['fields'][$i]['output'] = theme('query_display_rows' , $template_args);
    $i++;
  }
  return $rows;
}
/*
 * Make theme row classes
 */
function qw_row_classes($i){
  $row_classes   = array('query-row');
  $row_classes[] = ($i%2) ? 'query-row-odd' : 'query-row-even';
  $row_classes[] = 'query-row-'.$i;

  return implode(" ", $row_classes);
}
/*
 * Build array of fields and rows for templating
 *
 * @param object $new_query WP_Query objecte generated
 * @param array $display Query display data
 * @return array Executed query rows
 */
function qw_make_fields_rows(&$wp_query, $options)
{
  $display = $options['display'];
  $all_fields = qw_all_fields();
  $rows = array();
  $tokens = array();

  // loop through each post
  $i = 0;
  while($wp_query->have_posts())
  {
    $wp_query->the_post();
    //
    $this_post = $wp_query->post;
    $rows[$i] = array();

    // make row classes
    $rows[$i]['row_classes'] = qw_row_classes($i);

    if(is_array($display['field_settings']['fields']))
    {
      // sort according to weights
      uasort($display['field_settings']['fields'],'qw_cmp');

      // loop through each field
      foreach($display['field_settings']['fields'] as $field_name => $field_settings)
      {
        // field open
        $field_classes = array('query-field');
        $field_classes[] = 'query-field-'.$field_settings['name'];

        $rows[$i]['fields'][$field_name]['classes'] = implode(" ",$field_classes);

        // get field details from all fields list
        $field_defaults = $all_fields[$field_settings['type']];

        // merge default data with values
        $field = array_merge($field_defaults, $field_settings);

        // look for callback
        if(isset($field_defaults['output_callback']))
        {
          // callbacks with token arguments
          if ($field_defaults['output_arguments']){
            $rows[$i]['fields'][$field_name]['output'].= $field_defaults['output_callback']($this_post, $field);
          }
          // normal callback w/o arguments
          else {
            $rows[$i]['fields'][$field_name]['output'].= $field_defaults['output_callback']();
          }
        }
        // use field itself
        else {
          $rows[$i]['fields'][$field_name]['output'].= $this_post->{$field_settings['type']};
        }

        // apply link to field
        if(isset($field_settings['link'])){
          $rows[$i]['fields'][$field_name]['output'] = '<a class="query-field-link" href="'.get_permalink().'">'.$rows[$i]['fields'][$field_name]['output'].'</a>';
        }

        // add token for replace
        $tokens['{{'.$field_name.'}}'] = $rows[$i]['fields'][$field_name]['output'];

        // look for rewrite output
        if(isset($field_settings['rewrite_output'])){
          // replace tokens with results
          $field_settings['custom_output'] = str_replace(array_keys($tokens), array_values($tokens), $field_settings['custom_output']);
          $rows[$i]['fields'][$field_name]['output'] = $field_settings['custom_output'];
        }

        // get default field label for tables
        $rows[$i]['fields'][$field_name]['label'] = ($field_settings['has_label']) ? $field_settings['label'] : '';

        // apply labels to full style fields
        if(isset($field_settings['has_label']) &&
           $display['type'] != 'full' &&
           $display['field_settings']['style'] != 'table')
        {
          $rows[$i]['fields'][$field_name]['output'] = '<label class="query-label">'.$field_settings['label'].'</label> '.$rows[$i]['fields'][$field_name]['output'];
        }

        // apply shortcodes to field output
        $rows[$i]['fields'][$field_name]['output'] = do_shortcode($rows[$i]['fields'][$field_name]['output']);

        // after all operations, remove if excluded
        if($field_settings['exclude_display']){
          unset($rows[$i]['fields'][$field_name]['output']);
        }
      }
    }
    // increment row
    $i++;
  }

  return $rows;
}

/*
 * Custom Pager function
 *
 * @param array $pager Query pager details
 * @param object $wp_query Object
 * @return HTML processed pager
 */
function qw_make_pager($pager, &$wp_query)
{
  $pager_themed = '';
  $pagers = qw_all_pager_types();

  //set callback if function exists
  if(function_exists($pagers[$pager['type']]['callback'])) {
    $callback = $pagers[$pager['type']]['callback'];
  } else {
    $callback = $pagers['default']['callback'];
  }

  // execute callback
  $pager_themed = call_user_func_array($callback, array($pager, $wp_query));

  return $pager_themed;
}