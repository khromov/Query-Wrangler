<?php
// hook into qw_basics
add_filter('qw_basics', 'qw_basic_settings_posts_per_page');

/*
 * Basic Settings
 */
function qw_basic_settings_posts_per_page($basics){
  $basics['posts_per_page'] = array(
    'title' => 'Posts Per Page',
    'type' => 'args',
    'description' => 'Number of posts to show per page. Use -1 to display all results.',
    'form_callback' => 'qw_basic_settings_posts_per_page_form',
    'weight' => 0,
  );

  return $basics;
}

function qw_basic_settings_posts_per_page_form($basic, $options)
{
  ?>
    <input class="qw-text-short"
           type="text"
           name="<?php print $basic['form_prefix']; ?>[posts_per_page]"
           value="<?php print $otions['args']['posts_per_page']; ?>" />
    <p class="description"><?php print $basic['description']; ?></p>
  <?php
}