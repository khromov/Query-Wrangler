<?php
$_SESSION['qw']['time']['preview']['start'] = microtime(1);

global $wp_query;
$temp = $wp_query;
$wp_query = NULL;

ob_start();

$_SESSION['qw']['time']['execute']['start'] = microtime(1);
  // get the query options, force override
  $options = qw_generate_query_options($query_id, $options, true);

  do_action_ref_array('qw_pre_preview', array(&$options));

  // get formatted query arguments
  $args = qw_generate_query_args($options);
  // set the new query
  $wp_query = new WP_Query($args);

$_SESSION['qw']['time']['execute']['end'] = microtime(1);

$_SESSION['qw']['time']['template']['start'] = microtime(1);
  // get the themed content
  print qw_template_query($wp_query, $options);
$_SESSION['qw']['time']['template']['end'] = microtime(1);
$preview = ob_get_clean();

// args
$args = "<pre>".print_r($args, true)."</pre>";

// display
$display = "<pre>".htmlentities(print_r($options['display'], true))."</pre>";

$new_query = "<pre>".print_r($wp_query,true)."</pre>";

$templates = "These template files will be searched for relative to your theme folder.<br />
              To override a query's template, copy the corresponding template from the <span style='font-family: monospace;'>query-wrangler/templates</span> folder to your theme folder (or THEME/templates) and rename it.
              <pre>".print_r($templates,1)."</pre>";
// return
$preview = array(
  'preview' => $preview,
  'args' => $args,
  'display' => $display,
  'wpquery' => $new_query,
  'templates' => $templates,
);

do_action_ref_array('qw_post_preview', array(&$preview));

$_SESSION['qw']['time']['preview']['end'] = microtime(1);

$preview['time'] = qw_debug_query_time();

print json_encode($preview);