<?php
/*
 * $contextual_filter: array of default $contextual_filter data, and $contextual_filter['values']
 */
?>
<!-- <?php print $contextual_filter['name']; ?> -->
<div id="qw-contextual_filter-<?php print $contextual_filter['name']; ?>" class="qw-contextual_filter qw-sortable-item qw-item-form">
  <span class="qw-setting-header">
    <?php print $contextual_filter['title']; ?>
  </span>
  <div class="group">
    <input class='qw-contextual_filter-type'
           type='hidden'
           name='<?php print $contextual_filter['form_prefix']; ?>[type]'
           value='<?php print $contextual_filter['type']; ?>' />
    <input class='qw-contextual_filter-hook_key'
           type='hidden'
           name='<?php print $contextual_filter['form_prefix']; ?>[hook_key]'
           value='<?php print $contextual_filter['hook_key']; ?>' />
    <input class='qw-contextual_filter-name'
           type='hidden'
           name='<?php print $contextual_filter['form_prefix']; ?>[name]'
           value='<?php print $contextual_filter['name']; ?>' />

    <div class="qw-remove button">
      Remove
    </div>
    <input class='qw-weight'
           name='<?php print $contextual_filter['form_prefix']; ?>[weight]'
           type='text' size='2' tabindex="9999"
           value='<?php print $contextual_filter['weight']; ?>' />

    <p class="description"><?php print $contextual_filter['description']; ?></p>

    <div class="qw-setting">
      <label class="qw-label">Default context:</label>
      <select name='<?php print $contextual_filter['form_prefix']; ?>[context]'>
        <option>None</option>
        <?php
          $contexts = qw_all_contexts();
          foreach($contexts as $value => $context) {
            $selected = ($contextual_filter['values']['context'] == $value) ? 'selected="selected"' : '';
            ?>
              <option value="<?php print $value; ?>" <?php print $selected; ?> ><?php print $context['title']; ?></option>
            <?php
          }
        ?>
      </select>
    </div>

    <div>
      <?php
        if ($contextual_filter['can_override'])
        {
          $do_override = ($contextual_filter['values']['do_override']) ? 'checked="checked"': '';
          ?>
          <label class="qw-label">
            Override pages:
          </label>
          <input name="<?php print $contextual_filter['form_prefix']; ?>[do_override]"
                 type="checkbox"
                 <?php print $do_override; ?> />
          <p class="description clear-left">If checked, selected <?php print $contextual_filter['title']; ?>'s pages will be overridden by this query's display.</p>
          <?php
        }
      ?>

      <?php
        if ($contextual_filter['form'])
        { ?>
          <div class="qw-contextual_filter-form">
            <?php print $contextual_filter['form']; ?>
          </div>
          <?php
        }
      ?>
    </div>
  </div>
</div>
