<?php
add_action('qw_contexts', 'qw_contexts_default');

function qw_contexts_default($contexts){
  $contexts['post_id'] = array(
    'title' => 'Single Post being viewed',
  );

  $contexts['query_vars'] = array(
    'title' => 'Query vars',
  );

  return $contexts;
}