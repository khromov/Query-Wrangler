<!-- <?php print $handler['title']; ?> -->
  <div id="qw-display-add-<?php print $handler['hook_key']; ?>" class="qw-hidden">
    <input class="add-handler-type" type="hidden" value="<?php print $handler['hook_key']; ?>">
    <p class="description"><?php print $handler['description']; ?></p>
    <div class="qw-checkboxes">
      <?php
        // loop through sorts
        foreach($handler['all_items'] as $hook_key => $item)
        {
          ?>
          <label class="qw-sort-checkbox">
            <input type="checkbox"
                   value="<?php print $item['type']; ?>" />
            <input class="qw-hander-hook_key"
                   type="hidden"
                   value="<?php print $item['hook_key']; ?>" />
            <?php print $item['title']; ?>
          </label>
          <p class="description qw-desc"><?php print $item['description']; ?></p>
          <?php
        }
      ?>
    </div>
  </div>