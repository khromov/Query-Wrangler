<?php
// hook into qw_basics
add_filter('qw_basics', 'qw_basic_settings_footer');

/*
 * Basic Settings
 */
function qw_basic_settings_footer($basics){
  $basics['footer'] = array(
    'title' => 'Footer',
    'type' => 'display',
    'description' => 'The content placed here will appear below the resulting query.',
    'form_callback' => 'qw_basic_settings_footer_form',
    'weight' => 0,
  );
  return $basics;
}

function qw_basic_settings_footer_form($basic, $options)
{
  ?>
    <p class="description"><?php print $basic['description']; ?></p>
    <textarea name="<?php print $basic['form_prefix']; ?>[footer]"
              class="qw-field-textarea"><?php print qw_textarea($options['display']['footer']); ?></textarea>
  <?php
}