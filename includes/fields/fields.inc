<?php
// add default fields to the hook filter
add_filter('qw_fields', 'qw_fields_default');

/*
 * Some fields are very generic and easy to make, those are included here
 */
function qw_fields_default($fields)
{
  $fields['ID'] = array(
    'title' => 'Post ID',
    'description' => 'The post ID.',
  );
  $fields['post_title'] = array(
    'title' => 'Post Title',
    'description' => 'The title of a post.',
    'output_callback' => 'get_the_title',
  );
  $fields['post_content'] = array(
    'title' => 'Post Content',
    'description' => 'The full content body of a post.',
    'output_callback' => 'get_the_content',
  );
  $fields['post_excerpt'] = array(
    'title' => 'Post Excerpt',
    'description' => 'The excerpt of a post.',
    'output_callback' => 'get_the_excerpt',
  );
  $fields['post_date'] = array(
    'title' => 'Post Date',
    'description' => 'Published date of a post.',
    'output_callback' => 'get_the_date',
  );
  $fields['post_status'] = array(
    'title' => 'Post Status',
    'description' => 'Status of a post.',
  );
  $fields['post_parent'] = array(
    'title' => 'Post Parent',
    'description' => 'Parent page ID for a page.',
  );
  $fields['post_modified'] = array(
    'title' => 'Post Modified',
    'description' => 'Last date a post was modified.',
  );
  $fields['guid'] = array(
    'title' => 'GUID',
    'description' => 'Global Unique ID for a post (url).',
  );
  $fields['post_type'] = array(
    'title' => 'Post Type',
    'description' => 'The type of a post.',
  );
  $fields['comment_count'] = array(
    'title' => 'Comment Count',
    'description' => 'Number of comments for a post.',
  );
  $fields['permalink'] = array(
    'title' => 'Permalink',
    'description' => 'Pretty URL for a post.',
    'output_callback' => 'get_permalink',
  );

  return $fields;
}
