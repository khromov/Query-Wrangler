<!-- <?php print $handler['title']; ?>s -->
  <div id="tabs-<?php print $handler['hook_key']; ?>" class="qw-query-admin-tabs">
    <div class="qw-query-add-titles qw-setting">
      <span class="qw-query-title button" title="qw-display-add-<?php print $handler['hook_key']; ?>">
        Add <?php print $handler['title']; ?>s
      </span>
    </div>
    <p class="description">Click to change settings.  Drag and drop the fields to change their order.</p>

    <!-- edit fields -->
    <div id="existing-<?php print $handler['hook_key']; ?>" class="qw-sortable-list">
      <?php
        if(is_array($handler['items']))
        {
          $tokens = array();
          // loop through existing fields
          foreach($handler['items'] as $hook_key => $item)
          {
            $args = array(
              $handler['hook_key'] => $item,
            );

            // special case for fields
            if ($handler['hook_key'] == 'field'){
              $tokens[$item['name']] = '{{'.$item['name'].'}}';
              $args['tokens'] => $tokens;
            }

            print theme('query_'.$handler['hook_key'], $args);
          }
        }
        else
        { ?>
          <div class="qw-empty-list ui-state-highlight ui-corner-all">
            No fields yet.  Click 'Add Fields' at the top to begin.
          </div>
          <?php
        }
      ?>
    </div>
  </div>
      <!-- /edit <?php print $handler['hook_key']; ?>s -->