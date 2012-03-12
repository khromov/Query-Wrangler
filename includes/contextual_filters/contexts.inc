<?php
add_action('qw_contexts', 'qw_contexts_default');

function qw_contexts_default($contexts){
  $contexts['none'] = array(
    'title' => 'None',
    'description' => 'Do nothing.',
  );

  $contexts['single'] = array(
    'title' => 'Single',
    'description' => 'Get values of this filter from a single page or post being viewed.',
  );

  $contexts['global_query'] = array(
    'title' => 'Global Query',
    'description' => 'Get values of this filter from the global $wp_query object.  Use this to get the category when viewing a category page.',
  );

  return $contexts;
}

/*
 CF = contextual filter

post_id :
get's information from a single post or page being views.

- Taxonomies CF pulls a tax's terms from the single post being viewed
  and makes them into filters for the query
- Post Parent CF pull's in the post_parent of the single post/page being viewed
  and set's the post_parent arg of the query to this value


query_vars :
looks for the relavent information in the global WP_Query

- Taxonomies CF pulls in info from th global WP_Query, and passes it along to this query
- Post Parent CF pulls in post_parent of global WP_Query, and alters this query