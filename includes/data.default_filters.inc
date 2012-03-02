<?php
// http://codex.wordpress.org/Function_Reference/get_taxonomies

/*
 * Default Filters
 *
 * Template

  $filters['hook_key'] = array(
    // title shown when selecting a filter
    'title' => 'Title',

    // (optional) the query argument key
    // if doesn't exist, defaults to the hook_key
    'type' => 'filter_type',

    // help text for the user
    'description' => 'Describe this filter',

    // which types of queries to allow this filter on
    // some filters don't make sense on some query types
    'query_display_types' => array('page','widget','override'),

    // (optional) callback for form
    // * This or a template must be used
    // callback function should actually output the form
    'form_callback' => 'callback_function_form_inner_form',

    // (optional) template
    // template wrangler theme function or template file
    'form_template' => 'my_tw_template_hook',

    // (optional) generate_args callback
    // determines how form data becomes WP_Query arguments
    // defaults to form key as WP_Query argument key
    'query_args_callback' => 'qw_generate_query_args_post_types',

    // (optional) process the exposed filter's values into the query args
    // @param &$args - the arguments for the WP_Query
    // @param $filter -
    'exposed_process' => 'qw_filter_exposed_process_post_id',

    // (optional) a form for gather settings for the exposed filter
    // expects html output
    // @param $filter
    'exposed_settings_form' => 'qw_filter_exposed_settings_form_post_id',

    // (optional) the form exposed to a user above the query
    //  expects html output
    // @param $filter -
    'exposed_form' => 'qw_filter_exposed_form_post_id',
  );

 */
function qw_default_filters($filters){

  $filters['post_types'] = array(
    'title' => 'Post Type',
    'description' => 'Select which post types should be shown.',
    'form_callback' => 'qw_filter_post_types',
    'query_args_callback' => 'qw_generate_query_args_post_types',
    'query_display_types' => array('page','widget','override'),
  );
  $filters['post_parent'] = array(
    'title' => 'Post Parent',
    'description' => 'Use only with post type "Page" to show results with the chosen parent ID.',
    'form_callback' => 'qw_filter_post_parent',
    'query_display_types' => array('page','widget'),
  );
  $filters['post_id'] = array(
    'title' => 'Post IDs',
    'description' => 'Provide a list of post_ids to show or not show.',
    'form_callback' => 'qw_filter_post_id',
    'query_args_callback' => 'qw_generate_query_args_post_id',
    'query_display_types' => array('page','widget','override'),
    // exposed
    'exposed_process' => 'qw_filter_exposed_process_post_id',
    'exposed_settings_form' => 'qw_filter_exposed_settings_form_post_id',
    'exposed_form' => 'qw_filter_exposed_form_post_id',
  );
  $filters['categories'] = array(
    'title' => 'Categories',
    'description' => 'Select which categories to pull posts from, and how to treat those categories.',
    'form_callback' => 'qw_filter_categories',
    'query_args_callback' => 'qw_generate_query_args_categories',
    'query_display_types' => array('page','widget'),
  );
  $filters['tags'] = array(
    'title' => 'Tags',
    'description' => 'Select which tags to use.',
    'form_callback' => 'qw_filter_tags',
    'query_args_callback' => 'qw_generate_query_args_tags',
    'query_display_types' => array('page','widget'),
  );
  $filters['meta_key_value'] = array(
    'title' => 'Meta Key/Value Compare',
    'description' => 'Filter for a specific meta_key / meta_value pair.',
    'form_callback' => 'qw_filter_meta_key_value',
    'query_args_callback' => 'qw_generate_query_args_meta_key_value',
    'query_display_types' => array('page','widget','override'),
    // exposed
    'exposed_process' => 'qw_filter_exposed_process_meta_key_value',
    'exposed_settings_form' => 'qw_filter_exposed_settings_form_meta_key_value',
    'exposed_form' => 'qw_filter_exposed_form_meta_key_value',
  );
  $filters['meta_key'] = array(
    'title' => 'Meta Key',
    'description' => 'Filter for a specific meta_key.',
    'form_callback' => 'qw_filter_meta_key',
    'query_args_callback' => 'qw_generate_query_args_meta_key',
    'query_display_types' => array('page','widget','override'),
    // exposed
    'exposed_process' => 'qw_filter_exposed_process_meta_key',
    'exposed_settings_form' => 'qw_filter_exposed_settings_form_meta_key',
    'exposed_form' => 'qw_filter_exposed_form_meta_key',
  );
  $filters['meta_value'] = array(
    'title' => 'Meta Value',
    'description' => 'Filter for a specific meta_value.',
    'form_callback' => 'qw_filter_meta_value',
    'query_args_callback' => 'qw_generate_query_args_meta_value',
    'query_display_types' => array('page','widget','override'),
    // exposed
    'exposed_process' => 'qw_filter_exposed_process_meta_value',
    'exposed_settings_form' => 'qw_filter_exposed_settings_form_meta_value',
    'exposed_form' => 'qw_filter_exposed_form_meta_value',
  );
  return $filters;
}
// add default filters to the filter
add_filter('qw_filters', 'qw_default_filters');

/*
 * Post IDs exposed
 */
function qw_filter_exposed_process_post_id(&$args, $filter){
  $values = $_GET[$filter['exposed_key']];
  if(!empty($values)){
    $args[$filter['compare']] = explode(",", $values);
  }
}
function qw_filter_exposed_form_post_id($filter)
{ ?>
  <input type="text"
         name="<?php print $filter['exposed_key']; ?>"
         value="<?php print $_GET[$filter['exposed_key']]; ?>" />
  <?php
}
function qw_filter_exposed_settings_form_post_id($filter){}

/*
 * Meta values exposed
 */
function qw_filter_exposed_process_meta_value(&$args, $filter){
  $values = $_GET[$filter['exposed_key']];
  if(!empty($values)){
    $args[$filter['compare']] = explode(",", $values);
  }
}
function qw_filter_exposed_form_meta_value($filter)
{ ?>
  <input type="text"
         name="<?php print $filter['exposed_key']; ?>"
         value="<?php print $_GET[$filter['exposed_key']]; ?>" />
  <?php
}
function qw_filter_exposed_settings_form_meta_value($filter){}

/*
 * Meta key exposed
 */
function qw_filter_exposed_process_meta_key(&$args, $filter){
  $values = $_GET[$filter['exposed_key']];
  if(!empty($values)){
    $args[$filter['compare']] = $values;
  }
}
function qw_filter_exposed_form_meta_key($filter)
{ ?>
  <input type="text"
         name="<?php print $filter['exposed_key']; ?>"
         value="<?php print $_GET[$filter['exposed_key']]; ?>" />
  <?php
}
function qw_filter_exposed_settings_form_meta_key($filter){}

/************************************************************************************************
/*
 * Filter's query_args_callbacks
 * These functions convert form data into WP_Query arguments
 */
function qw_generate_query_args_post_types(&$args, $filter){
  $args['post_type'] = $filter['post_types'];
}
function qw_generate_query_args_post_id(&$args, $filter){
  $pids = explode(",", $filter['post_ids']);
  array_walk($pids, 'qw_trim');
  $args[$filter['compare']] = $pids;
}
function qw_generate_query_args_categories(&$args, $filter){
  if($filter['cat_operator'] == 'cat' && is_array($filter['cats'])){
    $args[$filter['cat_operator']] = implode(",", array_keys($filter['cats']));
  }
  else {
    $args[$filter['cat_operator']] = $filter['cats'];
  }
}
/*
 * Forms and Argument Callbacks
 *
 *   $filter: Combined filter data and values saved
 *   $filter['values']: Saved values for this filter
 *   $filter['form_prefix']: beginning of a form item's name attribute
 *
 * Forms: return form output
 * Argument Callbacks: return the entire output of the filter
 *
 *   $filter: filter values
 */
function qw_generate_query_args_tags(&$args, $filter){
  if(is_array($filter['tags'])){
    $args[$filter['tag_operator']] = array_keys($filter['tags']);
  }
}
function qw_generate_query_args_meta_key_value(&$args, $filter){
  $args['meta_key'] = $filter['meta_key'];
  $args['meta_value'] = stripslashes($filter['meta_value']);
  $args['meta_compare'] = $filter['meta_compare'];
}
function qw_generate_query_args_meta_value(&$args, $filter){
  $args['meta_value'] = stripslashes($filter['meta_value']);
}

/*
 * Post Types
 * - checkboxes
 */
function qw_filter_post_types($filter)
{ ?>
  <div class="qw-checkboxes">
    <?php
      $post_types = qw_all_post_types();
      // loop through post types
      foreach ($post_types  as $post_type )
      {
        $post_type_checked = (isset($filter['values']['post_types'][$post_type])) ? 'checked="checked"' : '';
        ?>
        <label class="qw-query-checkbox">
          <input type="checkbox"
                 name="<?php print $filter['form_prefix']; ?>[post_types][<?php print $post_type; ?>]"
                 value="<?php print $post_type; ?>"
                 <?php print $post_type_checked; ?> />
          <?php print ucfirst($post_type); ?>
        </label>
        <?php
      }
    ?>
  </div>
  <?php
}

/*
 * Post Id
 * - text, select
 */
function qw_filter_post_id($filter)
{ ?>
    <input type='text' name="<?php print $filter['form_prefix']; ?>[post_ids]" value='<?php print $filter['values']['post_ids']; ?>' />
    <p class="description">How to treat these post IDs.</p>

    <p>
      <select name="<?php print $filter['form_prefix']; ?>[compare]">
        <option value="post__in" <?php if($filter['values']['compare'] == "post__in") { print 'selected="selected"';} ?>>Only these posts</option>
        <option value="post__not_in" <?php if($filter['values']['compare'] == "post__not_in") { print 'selected="selected"';} ?>>Not these posts</option>
      </select>
    </p>
  <?php
}

/*
 * Post Parent
 * - text
 */
function qw_filter_post_parent($filter)
{ ?>
  <input class="qw-field-value"
         type="text"
         name="<?php print $filter['form_prefix']; ?>[post_parent]"
         value="<?php print $filter['values']['post_parent']; ?>" />
  <?php
}

/*
 * Categories
 * - checkboxes, select
 */
function qw_filter_categories($filter)
{ ?>
  <div class="qw-checkboxes">
    <?php
      $category_ids = get_all_category_ids();
      // List all categories as checkboxes
      foreach($category_ids as $cat_id)
      {
        $cat_name = get_cat_name($cat_id);
        $cat_checked = (isset($filter['values']['cats'][$cat_id])) ? 'checked="checked"' : '';
        ?>
        <label class="qw-query-checkbox">
          <input type="checkbox"
                 name="<?php print $filter['form_prefix']; ?>[cats][<?php print $cat_id; ?>]"
                 value="<?php print $cat_name; ?>"
                 <?php print $cat_checked; ?> />
          <?php print $cat_name; ?>
        </label>
        <?php
      }
    ?>
  </div>
  <p><strong>Categories Options</strong> - show posts that are:</p>
  <select name="<?php print $filter['form_prefix']; ?>[cat_operator]" class="qw-field-value">
    <option value="cat" <?php if($filter['values']['cat_operator'] == "cat") {print 'selected="selected"';} ?>>Any category plus children categories</option>
    <option value="category__in" <?php if($filter['values']['cat_operator'] == "category__in") {print 'selected="selected"';} ?>>Any category without children categories</option>
    <option value="category__and" <?php if($filter['values']['cat_operator'] == "category__and") {print 'selected="selected"';} ?>>All categories selected</option>
    <option value="category__not_in" <?php if($filter['values']['cat_operator'] == "category__not_in") {print 'selected="selected"';} ?>>Not in the categories selected</option>
  </select>
  <?php
}

/*
 * Tags
 * - checkboxes, select
 */
function qw_filter_tags($filter)
{ ?>
  <div class="qw-checkboxes">
    <?php
      $tags = get_tags(array('hide_empty' => false));
      foreach($tags as $tag)
      {
        $tag_checked = (isset($filter['values']['tags'][$tag->term_id])) ? 'checked="checked"' : '';
        ?>
        <label class="qw-query-checkbox">
          <input type="checkbox"
                 name="<?php print $filter['form_prefix']; ?>[tags][<?php print $tag->term_id; ?>]"
                 value="<?php print $tag->name; ?>"
                 <?php print $tag_checked; ?> />
          <?php print $tag->name; ?>
        </label>
        <?php
      }
    ?>
  </div>
  <p><strong>Tag Options</strong> - show posts that have:</p>
  <select name="<?php print $filter['form_prefix']; ?>[tag_operator]" class="qw-field-value">
    <option value="tag__in">Any of the selected tags</option>
    <option value="tag__and">All of the selected tags</option>
    <option value="tag__not_in">None of the selected tags</option>
  </select>
  <?php
}

/*
 * Meta Key / Value Pair
 * - text, select, text
 */
function qw_filter_meta_key_value($filter)
{ ?>
    <label class="qw-label">Meta Key:</label>
    <input type='text' name="<?php print $filter['form_prefix']; ?>[meta_key]" value='<?php print $filter['values']['meta_key']; ?>' />

    <p>
      <label class="qw-label">Compare:</label>
      <select name="<?php print $filter['form_prefix']; ?>[meta_compare]">
        <option value="="  <?php if($filter['values']['meta_compare'] == "=")  {print 'selected="selected"';} ?>>Is equal to</option>
        <option value="!=" <?php if($filter['values']['meta_compare'] == "!=") {print 'selected="selected"';} ?>>Is not equal to</option>
        <option value="<"  <?php if($filter['values']['meta_compare'] == "<")  {print 'selected="selected"';} ?>>Is less than</option>
        <option value="<=" <?php if($filter['values']['meta_compare'] == "<=") {print 'selected="selected"';} ?>>Is less than or equal to</option>
        <option value=">"  <?php if($filter['values']['meta_compare'] == ">")  {print 'selected="selected"';} ?>>Is greater than</option>
        <option value=">=" <?php if($filter['values']['meta_compare'] == ">=") {print 'selected="selected"';} ?>>Is greater than or equal to</option>
      </select>
    </p>
    <p>
      <label class="qw-label">Meta Value:</label>
      <textarea name="<?php print $filter['form_prefix']; ?>[meta_value]"
                class="qw-meta-value"><?php print stripcslashes($filter['values']['meta_value']); ?></textarea>
    </p>
  <?php
}

/*
 * Meta Key
 * - text
 */
function qw_filter_meta_key($filter)
{ ?>
    <input type='text'
           name="<?php print $filter['form_prefix']; ?>[meta_key]"
           value='<?php print $filter['values']['meta_key']; ?>' />
  <?php
}

/*
 * Meta Value
 * - text
 */
function qw_filter_meta_value($filter)
{ ?>
    <textarea name="<?php print $filter['form_prefix']; ?>[meta_value]"
              class="qw-meta-value"><?php print stripcslashes($filter['values']['meta_value']); ?></textarea>
  <?php
}
