<?php
// hook into qw_all_filters()
add_filter('qw_filters', 'qw_filter_categories');

/*
 * Add filter to qw_filters
 */
function qw_filter_categories($filters){
  $filters['categories'] = array(
    'title' => 'Categories',
    'description' => 'Select which categories to pull posts from, and how to treat those categories.',
    'form_callback' => 'qw_filter_categories_form',
    'query_args_callback' => 'qw_filter_categories_args',
    'query_display_types' => array('page','widget'),
  );
  return $filters;
}

/*
 * Convert values into query args
 */
function qw_filter_categories_args(&$args, $filter){
  if($filter['cat_operator'] == 'cat' && is_array($filter['cats'])){
    $args[$filter['cat_operator']] = implode(",", array_keys($filter['cats']));
  }
  else {
    $args[$filter['cat_operator']] = $filter['cats'];
  }
}

/*
 * Filter form
 */
function qw_filter_categories_form($filter)
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