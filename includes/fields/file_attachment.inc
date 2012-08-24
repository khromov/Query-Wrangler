<?php
// add default fields to the hook filter
add_filter('qw_fields', 'qw_field_file_attachment');
// add default file styles to the filter
add_filter('qw_file_styles', 'qw_file_styles_default', 0);

/*
 * Add field to qw_fields
 */
function qw_field_file_attachment($fields){
  $fields['file_attachment'] = array(
    'title' => 'File Attachment',
    'description' => 'Files that are attached to a post.',
    'output_callback' => 'qw_theme_file', // in theme.inc
    'output_arguments' => true,
    'form_callback' => 'qw_field_file_attachment_form',
  );
  return $fields;
}

/*
 * File Styles
 *
 * @return array of file styles
 */
function qw_file_styles_default($file_styles)
{
  $file_styles['link'] = array(
    'description' => 'Filename Link to File',
  );
  $file_styles['link_url'] = array(
    'description' => 'URL Link to File',
  );
  $file_styles['url'] = array(
    'description' => 'URL of File',
  );

  return $file_styles;
}

/*
 * File attachment settings
 */
function qw_field_file_attachment_form($field)
{
  $file_styles = qw_all_file_styles();
  ?>
    <!-- file display -->
    <label class="qw-label">Number of items to show:</label>
    <input class="qw-text-short qw-title-ignore"
           type="text"
           name='<?php print $field['form_prefix']; ?>[file_display_count]'
           value="<?php print ($field['values']['file_display_count']) ? $field['values']['file_display_count']: 0;?>" />
    <p class="description">Number of files to show.  Use 0 (zero) to show all attached files.</p>
    <label class="qw-label">Starting item:</label>
    <input class="qw-text-short qw-title-ignore"
           type="text"
           name='<?php print $field['form_prefix']; ?>[file_display_start]'
           value="<?php print ($field['values']['file_display_start']) ? $field['values']['file_display_start']: 1;?>" />
    <p class="description">Number representing the file to start with.</p>

    <div>
      <label class="qw-label">File Display Style:</label>
      <select name='<?php print $field['form_prefix']; ?>[file_display_style]'>
        <?php
          foreach($file_styles as $key => $file_style_details)
          {
            $style_selected = ($field['values']['file_display_style'] == $key) ? 'selected="selected"' : '';
            ?>
              <option value="<?php print $key; ?>" <?php print $style_selected; ?>><?php print $file_style_details['description']; ?></option>
            <?php
          }
        ?>
      </select>
      <p class="description">Select how the file should be displayed.</p>
    </div>
  <?php
}

/*
 * Get and theme attached post files
 *
 * @param int $post_id The post->ID
 * $param int $count Number of files to get
 */
function qw_theme_file($post, $field){
  $style = ($field['values']['file_display_style']) ? $field['values']['file_display_style'] : 'link';
  $max_count = ($field['values']['file_display_count']) ? $field['values']['file_display_count'] : 0;
  $start = ($field['values']['file_display_start']) ? $field['values']['file_display_start'] : 1;

  $files = qw_get_post_files($post->ID);
  if(is_array($files))
  {
    $output = '';
    $i = 1;
    $count = 0;
    foreach($files as $file){
      // make sure we only output items after the 'start' number
      if ($i >= $start){
        // not images
        if (substr($file->post_mime_type,0,5) != "image"){
          // makesure we're less than the total items requested
          if($max_count == 0 || ($count < $max_count))
          {
            switch($style){
              case 'url':
              $output .= '<div class="file-attachement">'.wp_get_attachment_url($file->ID).'</div>';
              break;

              case 'link':
              // complete file name
              $file_name = explode("/", $file->guid);
              $file_name = $file_name[count($file_name)-1];
              $output.= '<div class="file-attachement"><a href="'.wp_get_attachment_url($file->ID).'" class="query-file-link">'.$file_name.'</a></div>';
              break;

              case 'link_url':
                $output.= '<div class="file-attachement"><a href="'.wp_get_attachment_url($file->ID).'" class="query-file-link">'.$file->guid.'</a></div>';
                break;
            }
            $count++;
          }
        }
      }
      $i++;
    }
    return $output;
  }
}

/*
 * Get files attached to a post
 *
 * @param int $post_id The WP post id
 * @return Array of file posts
 */
function qw_get_post_files($post_id)
{
  $child_args = array(
    "post_type" => "attachment",
    "post_parent" => $post_id,
  );
		// Get images for this post
  $files = get_posts($child_args);

  if(is_array($files))
		{
    return $files;
  }
  return false;
}