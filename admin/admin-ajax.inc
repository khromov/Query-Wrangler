<?php
/*
 * Ajax form templates
 */
function qw_form_ajax()
{
  /*
   * Preview, special case
   */
  if ($_POST['form'] == 'preview'){
    $decode = urldecode($_POST['options']);
    $options = array();
    parse_str($decode, $options);
    $options['qw-query-options']['args']['paged'] = 1;
    $args = array(
      'options' => $options['qw-query-options'],
      'query_id' => $_POST['query_id'],
    );
    print theme('preview_ajax', $args);
    exit;
  }

  /*
   * Generate handler item forms and data
   */
  $handlers = qw_all_handlers();
  $handler = $handlers[$_POST['handler']];

  $item = array();

  $hook_key = qw_get_hook_key($handler['all_items'], $_POST); // kind of a trick
  $item = $handler['all_items'][$hook_key];

  $item['name'] = $_POST['name'];
  $item['form_prefix'] = $handler['form_prefix'].'['.$item['name'].']';

  if (isset($_POST['next_weight'])){
    $item['weight'] = $_POST['next_weight'];
  }

  if (!isset($item['form']) && function_exists($item['form_callback'])){
    ob_start();
      $item['form_callback']($item);
    $item['form'] = ob_get_clean();
  }

  $args = array(
    $_POST['handler'] => $item,
  );
  // weight for sortable handler items

  print theme($handler['wrapper_template'], $args);
  exit;
}
