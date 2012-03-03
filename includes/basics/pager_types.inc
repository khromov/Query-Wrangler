<?php
// add default pager types
add_filter('qw_pager_types', 'qw_pager_types_default', 0);

/*
 * Setup pager types
 */
function qw_pager_types_default($pagers) {
  $pagers['default'] = array(
    'title' => 'Default',
    'callback' => 'qw_theme_pager_default'
  );
  $pagers['numbers']	= array(
    'title' => 'Page Numbers',
    'callback' => 'qw_theme_pager_numbers'
  );

  // WP PageNavi Plugin
  if(function_exists('wp_pagenavi')) {
    $pagers['pagenavi'] = array(
      'title' => 'PageNavi',
      'callback' => 'wp_pagenavi'
    );
  }
  return $pagers;
}


/*
 * Custom Default Pager
 *
 * @param array $pager Query options for pager
 * @param object $wp_query Object
 */
function qw_theme_pager_default($pager, &$wp_query)
{
  // help figure out the current page
  $path_array = explode('/page/', $_SERVER['REQUEST_URI']);

  $pager_themed = '';
  $pager['next'] = ($pager['next']) ? $pager['next'] : 'Next Page &raquo;';
  $pager['previous'] = ($pager['previous']) ? $pager['previous'] : '&laquo; Previous Page';

  if($page = qw_get_page_number($wp_query))
  {
    $wpurl = get_bloginfo('wpurl');

    // previous link with page number
    if($page >= 3){
      $pager_themed.= '<div class="query-prevpage">
                        <a href="'.$wpurl.$path_array[0].'/page/'.($page-1).'">'.$pager['previous'].'</a>
                      </div>';
    }
    // previous link with no page number
    else if ($page == 2)
    {
      $pager_themed.= '<div class="query-prevpage">
                        <a href="'.$wpurl.$path_array[0].'">'.$pager['previous'].'</a>
                      </div>';
    }

    // next link
    if(($page+1) <= $wp_query->max_num_pages){
      $pager_themed.= '<div class="query-nextpage">
                        <a href="'.$wpurl.$path_array[0].'/page/'.($page+1).'">'.$pager['next'].'</a>
                      </div>';
    }

    return $pager_themed;
  }
}

/*
 * Default Pager with page numbers
 *
 * @param array $pager Query options for pager
 * @param object $wp_query Object
 *
 * @return string HTML for pager
 */
function qw_theme_pager_numbers($pager, $wp_query)
{
		$big = intval($wp_query->found_posts.'000');
		$args = array(
    'base' => str_replace( $big, '%#%', get_pagenum_link( $big ) ),
    'format' => '?paged=%#%',
    'current' => max( 1, get_query_var('paged') ),
    'total' => $wp_query->max_num_pages
		);
  $pager_themed = paginate_links($args);
  return $pager_themed;
}
