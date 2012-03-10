<?php
/*
 * $sort: array of default sort data, and specific sort data
 */
?>
<!-- <?php print $sort['name']; ?> -->
<div id="qw-sort-<?php print $sort['name']; ?>" class="qw-sort qw-sortable-item qw-item-form">
  <span class="qw-setting-header">
    <?php print $sort['title']; ?>
  </span>
  <div class="group">
    <input class='qw-sort-type'
           type='hidden'
           name='<?php print $sort['form_prefix']; ?>[type]'
           value='<?php print $sort['type']; ?>' />
    <input class='qw-sort-hook_key'
           type='hidden'
           name='<?php print $sort['form_prefix']; ?>[hook_key]'
           value='<?php print $sort['hook_key']; ?>' />
    <input class='qw-sort-name'
           type='hidden'
           name='<?php print $sort['form_prefix']; ?>[name]'
           value='<?php print $sort['name']; ?>' />

    <div class="qw-remove button">
      Remove
    </div>
    <input class='qw-weight'
           name='qw-query-options[args][sorts][<?php print $sort['name']; ?>][weight]'
           type='text' size='2'
           value='<?php print $sort['weight']; ?>' />

    <p class="description"><?php print $sort['description']; ?></p>

    <?php
      if ($sort['form'])
      { ?>
        <div class="qw-sort-form">
          <?php print $sort['form']; ?>
        </div>
        <?php
      }

      // exposed form and settings
      if(isset($sort['exposed_form']))
      {
        $is_exposed = ($sort['values']['is_exposed']) ? 'checked="checked"': '';
        ?>
        <div class="qw-exposed-form">
          <div class="qw-setting">
            <label class="qw-label">Expose Sort Option:</label>
            <p>
              <input type="checkbox"
                      name='<?php print $sort['form_prefix']; ?>[is_exposed]'
                      <?php print $is_exposed; ?> />
            </p>
            <p class="description">Exposing a sort option allows a site guest to alter the query results with a form.</p>
          </div>
          <div>
            <label class="qw-label">Exposed Label:</label>
            <input type="text"
                   name='<?php print $sort['form_prefix']; ?>[exposed_label]'
                   value="<?php print $sort['values']['exposed_label']; ?>" />
            <p class="description">Label for the exposed form item.</p>
          </div>
          <div>
            <label class="qw-label">Exposed Description:</label>
            <input class="qw-text-long"
                   type="text"
                   name='<?php print $sort['form_prefix']; ?>[exposed_desc]'
                   value="<?php print $sort['values']['exposed_desc']; ?>" />
            <p class="description">Useful for providing help text to a user.</p>
          </div>
          <div>
            <label class="qw-label">Exposed Key:</label>
            <input type="text"
                   name='<?php print $sort['form_prefix']; ?>[exposed_key]'
                   value="<?php print $sort['values']['exposed_key']; ?>" />
            <p class="description">URL ($_GET) key for the sort option.  Useful for multiple forms on a single page.</p>
          </div>
          <?php if ($sort['exposed_settings_form']){ ?>
            <div class="qw-exposed-settings-form">
              <?php print $sort['exposed_settings_form']; ?>
            </div>
          <?php } ?>
        </div>
        <?php
      }
    ?>
  </div>
</div>
