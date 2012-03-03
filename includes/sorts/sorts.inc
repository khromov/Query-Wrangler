<?php
// add default field styles to the filter
add_filter('qw_sort_options', 'qw_sorts_default');

/*
 * Sorts are generally very generic, so this one file includes all the defaults
 */
function qw_sorts_default($sort_options){

  $sort_options['author_id'] = array(
    'title' => 'Author',
    'description' => 'The content author ID.',
    'type' => 'author',
  );
  $sort_options['post_date'] = array(
    'title' => 'Date',
    'description' => 'The posted date of content.',
    'type' => 'date',
  );
  $sort_options['post_ID'] = array(
    'title' => 'Post ID',
    'description' => 'The ID of the content.',
    'type' => 'ID',
  );
  $sort_options['post_modified'] = array(
    'title' => 'Date Modified',
    'description' => 'Date content was last modified.',
    'type' => 'modified',
  );
  $sort_options['post_parent'] = array(
    'title' => 'Parent',
    'description' => 'The parent post for content.',
    'type' => 'parent',
  );
  $sort_options['post_title'] = array(
    'title' => 'Title',
    'description' => 'The title of the content.',
    'type' => 'title',
  );
  $sort_options['comment_count'] = array(
    'title' => 'Comment Count',
    'description' => 'Total number of comments on a piece of content.',
  );
  $sort_options['menu_order'] = array(
    'title' => 'Menu Order (for Page post_types)',
    'description' => 'Menu Order of a Page.',
  );
  $sort_options['none'] = array(
    'title' => 'None',
    'description' => 'No sort order.',
  );
  $sort_options['rand'] = array(
    'title' => 'Random',
    'description' => 'Random order.',
  );
  return $sort_options;
}

/*
 * Most sort options are just ASC and DESC, so we use a default
 * 
 * Default sort options 'order' options form
 */
function qw_sorts_default_form_callback($sort)
{ ?>
    <!-- sort options -->
    <label class="qw-label">Order by <?php print $sort['title']; ?>:</label>
    <select name="<?php print $sort['form_prefix']; ?>[order_value]">
      <?php
        foreach ($sort['order_options'] as $value => $label)
        { ?>
          <option value="<?php print $value; ?>"
                  <?php if($sort['values']['order_value'] == $value) { print 'selected="selected"';} ?>>
            <?php print $label; ?>
          </option>
          <?php
        }
      ?>
    </select>
    <p class="description">Select how to order the results.</p>
  <?php
}