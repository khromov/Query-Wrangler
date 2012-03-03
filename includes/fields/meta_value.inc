<?php
// add default fields to the hook filter
add_filter('qw_fields', 'qw_field_meta_value');

/*
 * Add field to qw_fields
 */
function qw_field_meta_value($fields){
  // add meta keys to field list
  $meta = qw_get_meta_keys();
  foreach($meta as $key){
    $field_key = 'meta_'.str_replace(" ", "_", $key);
    // ignore WP keys and Widget Wangler keys
    if(substr($key, 0, 1) != '_' &&
       substr($key,0,3) != 'ww-' &&
       substr($key,0,3) != 'ww_')
    {
      $fields[$field_key] = array(
        'title' => 'Custom Field: '.$key,
        'description' => 'WP Custom Field data with key: '.$key,
        'output_callback' => 'qw_get_post_meta',
        'output_arguments' => true,
        'meta_key' => $key,
      );
    }
  }
  return $fields;
}

/*
 * Output callback
 */
function qw_get_post_meta($post, $field){
  return get_post_meta($post->ID, $field['meta_key'], true);
}

/*
 * Function for grabbing meta keys
 *
 * @return array All meta keys in WP
 */
function qw_get_meta_keys() {
	global $wpdb;

	$keys = $wpdb->get_col( "
			SELECT meta_key
			FROM $wpdb->postmeta
			GROUP BY meta_key
			ORDER BY meta_key" );

	return $keys;
}