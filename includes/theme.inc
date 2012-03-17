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
      'templates/query-wrapper-[slug]-[type].php',
      'templates/query-wrapper-[slug].php',
      'templates/query-wrapper-[type].php',
      'templates/query-wrapper.php',
      'query-wrapper-[slug]-[type].php',
      'query-wrapper-[slug].php',
      'query-wrapper-[type].php',
      'query-wrapper.php',
    ),
    'default_path' => QW_PLUGIN_DIR,
    'arguments' => array(
      'type' => '',
      'slug' => '',
    )
  );
  // styles
  $templates['query_display_style'] = array(
    'files' => array(
      'templates/[template]-[slug]-[type].php',
      'templates/[template]-[slug].php',
      'templates/[template]-[type].php',
      'templates/[template].php',
      '[template]-[slug]-[type].php',
      '[template]-[slug].php',
      '[template]-[type].php',
      '[template].php',
    ),
    'default_path' => QW_PLUGIN_DIR,
    'arguments' => array(
      'template' => 'query-unformatted',
      'type' => '',
      'slug' => 'not-found',
      'style' => 'unformatted',
      'rows' => array(),
    )
  );
  // row styles
  $templates['query_display_rows'] = array(
    'files' => array(
      'templates/[template]-[slug]-[type].php',
      'templates/[template]-[slug].php',
      'templates/[template]-[type].php',
      'templates/[template].php',
      '[template]-[slug]-[type].php',
      '[template]-[slug].php',
      '[template]-[type].php',
      '[template].php',
    ),
    'default_path' => QW_PLUGIN_DIR,
    'arguments' => array(
      'template' => 'query-complete',
      'type' => '',
      'slug' => 'not-found',
      'row_style' => 'posts',
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
  $all_styles = qw_all_styles();
  // make sure we know what style to use
  if(isset($all_styles[$template['arguments']['style']]))
  {
    // get the specific style
    $style = $all_styles[$template['arguments']['style']];
    // set this template's default path to the style's default path
    if (!empty($style['default_path'])){
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
    'type' => $options['meta']['type'],
    'slug' => $options['meta']['slug'],
    'style' => $options['meta']['style'],
  );

  // look for empty results
  if ($results_count > 0)
  {
    // setup row template arguments
    $template_args = array(
      'template' => $options['meta']['template'],
      'type' => $options['meta']['type'],
      'slug' => $options['meta']['slug'],
      'style' => $options['meta']['style'],
      'style_settings' => $options['display']['style_settings'][$options['meta']['style']],
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
    $wrapper_args['content'] = theme('query_display_style', $template_args);
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
  $row_style_settings = $options['display']['row_style_settings'][$options['display']['row_style']];

  $i = 0;
  while($wp_query->have_posts())
  {
    $wp_query->the_post();
    $template_args = array(
      'template' => 'query-'.$row_style_settings['size'],
      'slug' => $options['meta']['slug'],
      'style' => $options['meta']['style'],
      'row_style' => $options['display']['row_style'],
      'type' => $options['meta']['type'],
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
  $row_classes[] = ($i%2) ? 'query-row-odd odd' : 'query-row-even even';
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

    if(is_array($display['fields']))
    {
      // sort according to weights
      uasort($display['fields'],'qw_cmp');

      // loop through each field
      foreach($display['fields'] as $field_name => $field_values)
      {
        // get field details from all fields list
        $field = $all_fields[$field_values['hook_key']];
        // merge default data with values
        $field['values'] = $field_values;

        // field open
        $field_classes = array('query-field');
        $field_classes[] = 'query-field-'.$field_name;

        $rows[$i]['fields'][$field_name]['classes'] = implode(" ",$field_classes);

        // look for callback
        if(isset($field['output_callback']))
        {
          // callbacks with token arguments
          if ($field['output_arguments']){
            $rows[$i]['fields'][$field_name]['output'].= $field['output_callback']($this_post, $field);
          }
          // normal callback w/o arguments
          else {
            $rows[$i]['fields'][$field_name]['output'].= $field['output_callback']();
          }
        }
        // use field itself
        else {
          $rows[$i]['fields'][$field_name]['output'].= $this_post->{$field['values']['type']};
        }

        // apply link to field
        if(isset($field['values']['link'])){
          $rows[$i]['fields'][$field_name]['output'] = '<a class="query-field-link" href="'.get_permalink().'">'.$rows[$i]['fields'][$field_name]['output'].'</a>';
        }

        // add token for replace
        $tokens['{{'.$field_name.'}}'] = $rows[$i]['fields'][$field_name]['output'];

        // look for rewrite output
        if(isset($field['values']['rewrite_output'])){
          // replace tokens with results
          $field['values']['custom_output'] = str_replace(array_keys($tokens), array_values($tokens), $field['values']['custom_output']);
          $rows[$i]['fields'][$field_name]['output'] = $field['values']['custom_output'];
        }

        // get default field label for tables
        $rows[$i]['fields'][$field_name]['label'] = ($field['values']['has_label']) ? $field['values']['label'] : '';

        // apply labels to full style fields
        if(isset($field['values']['has_label']) &&
           $display['style'] != 'table')
        {
          $rows[$i]['fields'][$field_name]['output'] = '<label class="query-label">'.$field['values']['label'].'</label> '.$rows[$i]['fields'][$field_name]['output'];
        }

        // apply shortcodes to field output
        $rows[$i]['fields'][$field_name]['output'] = do_shortcode($rows[$i]['fields'][$field_name]['output']);

        // after all operations, remove if excluded
        // do this late to allow the field to work properly if used as a token
        if ($field['values']['exclude_display']){
          unset($rows[$i]['fields'][$field_name]['output']);
        }
        else{
          $template_args = array(
            'template' => 'query-field',
            'slug' => $options['meta']['slug'],
            'style' => $options['meta']['style'],
            'row_style' => $options['display']['row_style'],
            'type' => $options['meta']['type'],
            'options' => $options,
            'row' => $rows[$i],
            'field' => $rows[$i]['fields'][$field_name],
            'output' => $rows[$i]['fields'][$field_name]['output'],
          );
          $rows[$i]['fields'][$field_name]['output'] = theme('query_display_rows' , $template_args);
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