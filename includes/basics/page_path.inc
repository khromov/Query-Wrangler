<?php
// hook into qw_basics
add_filter('qw_basics', 'qw_basic_settings_page_path',0);

/*
 * Basic Settings
 */
function qw_basic_settings_page_path($basics){
  $basics['page_path'] = array(
    'title' => 'Page Path',
    'type' => 'page',
    'description' => 'The path or permalink you want this page to use. Avoid using spaces and capitalization for best results.',
    'form_callback' => 'qw_basic_settings_page_path_form',
    'weight' => 0,
  );
  return $basics;
}


function qw_basic_settings_page_path_form($basic, $options)
{
  $page_enabled = ($options['display']['types']['page']) ? 'checked="checked"' : '';
  $path = $options['display']['page']['path'];
  ?>
  <div>
    <input size="60"
         type="text"
         name="<?php print $basic['form_prefix']; ?>[path]"
         value="<?php print $path; ?>" />
    <p class="description"><?php print $basic['description']; ?></p>
  </div>
  <?php
}