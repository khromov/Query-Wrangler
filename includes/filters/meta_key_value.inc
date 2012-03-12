<?php
// hook into qw_all_filters()
add_filter('qw_filters', 'qw_filter_meta_key_value');

/*
 * Add filter to qw_filters
 */
function qw_filter_meta_key_value($filters){
  $filters['meta_key_value'] = array(
    'title' => 'Meta Key/Value Compare',
    'description' => 'Filter for a specific meta_key / meta_value pair.',
    'form_callback' => 'qw_filter_meta_key_value_form',
    'query_args_callback' => 'qw_filter_meta_key_value_args',
  );
  return $filters;
}

/*
 * Convert values into query args
 */
function qw_filter_meta_key_value_args(&$args, $filter){
  $args['meta_key'] = $filter['values']['meta_key'];
  $args['meta_value'] = stripslashes($filter['values']['meta_value']);
  $args['meta_compare'] = $filter['values']['meta_compare'];
}

/*
 * Filter form
 */
function qw_filter_meta_key_value_form($filter)
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
