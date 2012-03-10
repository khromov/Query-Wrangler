<?php
// http://wordpress.org/support/topic/explanation-and-workaround-for-error-404-on-category-pagination?replies=3
// http://wordpress.org/extend/plugins/category-pagination-fix/

// this works for categories!
// taken from category-pagination-fix plugin and modified
function qw_remove_page_from_query_string($query_string)
{
  // fix tags
  if(isset($query_string['tag']) && $query_string['paged']){
    global $wp_query;
    $query_string['paged'] = qw_get_page_number($wp_query);
  }
  // fix categories
  if ($query_string['name'] == 'page' && isset($query_string['page'])) {
      unset($query_string['name']);
      unset($query_string['page']);
  }

  return $query_string;
}
add_filter('request', 'qw_remove_page_from_query_string');

// http://codex.wordpress.org/Conditional_Tags
function qw_get_page_op()
{
  global $wp_query;
  $op = '';

  // look for categories and tags in the query_vars
  if (is_category() || !empty($wp_query->query_vars['category_name'])){
    $op = 'category';
  }
  else if (is_tag() || !empty($wp_query->query_vars['tag'])){
    $op = 'tag';
  }
  else if (is_tax() || !empty($wp_query->query_vars['taxonomy'])){
    $op = 'tax';
  }
  else if (is_home()){
    $op = 'home';
  }
  else if (is_single()){
    // single is not pages
    $op = 'single';
  }
  else if (is_page()){
    $op = 'page';
  }
  else if (is_author()){
    $op = 'author';
  }
  else if (is_search()){
    $op = 'search';
  }
  else if (is_paged()){
    $op = 'paged';
  }
  else if (is_404()){
    $op = '404';
  }
  else {
    $op = 'default';
  }

  // fix incorrectly identifying query pages as categories or tags
  // depending on permalink structures
  if($op == 'category' ||
     $op == 'tag')
  {
    if ($wp_query->found_posts == 0){
      $op = '404';
    }
  }
  return $op;
}
/*
 * Query pages
 */
add_action('wp', 'qw_query_pages_handler');

/*
 * Handle Query pages
 * ref: http://www.blogseye.com/2010/05/creating-fake-wordpress-posts-on-the-fly/
 */
function qw_query_pages_handler()
{
  // don't override admin pages.... for now, whawhawha
  if (is_admin()){
    return;
  }

  global $wp_query,$wpdb;
  $handled_ops = array('category', 'tag', 'tax', '404');
  
  // page op is based on WP tags like is_category() && is_404()
  $op = qw_get_page_op();

  // we only handle sepcific types of pages and overrides
  if(in_array($op, $handled_ops))
  {
    $qw_table = $wpdb->prefix."query_wrangler";
    $qo_table = $wpdb->prefix."query_overrides";

    // We'll need to override query options
    $options_override = array();

    // Clean the result for DB
    $path = $wpdb->escape($wp_query->query['pagename']);

    // fix the query paging
    if($paged = qw_get_page_number($wp_query)){
      $wp_query->query_vars['paged'] = $paged;
      $wp_query->query_vars['page'] = $paged;
    }

    /*
     * 404 -> Query type = Page
     */
    if($op == '404'){
      // take into account trailing slash
      if(substr($path, strlen($path)-1, 1) == '/'){
        $path = rtrim($path, '/');
      }

      // Look for the query path given
      $sql = "SELECT id,name,path,data FROM ".$qw_table." WHERE type = 'page' AND path = '".$path."'";
      $query = $wpdb->get_row($sql);
    }

    /*
     * Category & Tag -> Query type = Override
     */
    // get query by contextual filter
    $handlers = qw_all_handlers();
    foreach ($handlers['contextual_filter']['all_items'] as $item){
      if ($item['query_op'] == $op){
        $override_id = $item['query_override_id_callback']($item);

        // get potential query
        $sql = "SELECT qw.id, qw.data
                FROM ".$qw_table." as qw
                LEFT JOIN ".$qo_table." as qo ON qo.query_id = qw.id
                WHERE qo.override_id = '".$override_id."'
                  AND qo.override_hook = '".$item['hook_key']."'
                LIMIT 1";
        if ($query = $wpdb->get_row($sql)){
          break;
        }
      }
    }

    // got results for a query
    if (isset($query))
    {
$_SESSION['qw']['time']['page_load']['start'] = microtime(1);
      // pass the paged value into the query
      $options_override['args']['paged'] = $paged;

      // actual options
      $options = qw_unserialize($query->data);

      // resulting query
      $themed_query = qw_execute_query($query->id, $options_override, false);

      // post title
      if($op == '404'){
        // The title of the query
        $post_title = ($options['display']['title']) ? $options['display']['title'] : $query->name;
      }
      else {
        // categories and tags use term name for title
        $post_title = $term->name;
      }

      // remove the autop filter to prevent paragraph randomly being added
      remove_filter( 'the_content', 'wpautop' );

      // Make the post object
      $post = new stdClass();
        $post->ID           = -42;  // Arbitrary post id
        $post->post_title   = $post_title;
        $post->post_content =  $themed_query;
        $post->post_status  = 'publish';
        $post->post_type    = 'page';
        $post->post_category= array('uncategorized');
        $post->post_excerpt = '';

      // set some query information
      $wp_query->queried_object = $post;
      $wp_query->post           = $post;
      $wp_query->found_posts    = true;
      $wp_query->post_count     = true;
      //$wp_query->max_num_pages = true;
      $wp_query->is_single      = true;
      $wp_query->is_posts_page  = true;
      $wp_query->is_page        = true;
      $wp_query->posts          = array($post);
      $wp_query->is_404         = false;
      $wp_query->is_post        = false;
      $wp_query->is_home        = false;
      $wp_query->is_archive     = false;
      $wp_query->is_category    = false;

$_SESSION['qw']['time']['page_load']['end'] = microtime(1);
$_SESSION['qw']['time']['page_template']['start'] = microtime(1);
      // According to http://codex.wordpress.org/Plugin_API/Action_Reference
      // we can safely exit here. The template will take care of the rest.
      // chosen template path
      $template_path = TEMPLATEPATH."/".$options['display']['page']['template-file'];
      if(!file_exists($template_path)) {
      	$template_path = TEMPLATEPATH."/".qw_default_template_file();
      }

      include($template_path);
$_SESSION['qw']['time']['page_template']['end'] = microtime(1);
//print qw_debug_query_time();
      exit();
    }
  }
}
