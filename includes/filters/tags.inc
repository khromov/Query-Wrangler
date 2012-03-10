<?php
// hook into qw_all_filters()
add_filter('qw_filters', 'qw_filter_tags');

/*
 * Add filter to qw_filters
 */
function qw_filter_tags($filters){
  $filters['tags'] = array(
    'title' => 'Tags',
    'description' => 'Select which tags to use.',
    'form_callback' => 'qw_filter_tags_form',
    'query_args_callback' => 'qw_filter_tags_args',
    'allowed_query_types' => array('page','widget'),
  );
  return $filters;
}

/*
 * Convert values into query args
 */
function qw_filter_tags_args(&$args, $filter){
  if(is_array($filter['values']['tags'])){
    $args[$filter['values']['tag_operator']] = array_keys($filter['values']['tags']);
  }
}

/*
 * Filter form
 */
function qw_filter_tags_form($filter)
{
  $tags = get_tags(array('hide_empty' => false));
  ?>
  <div class="qw-checkboxes">
    <?php
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
