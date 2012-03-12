<?php
/*
 * Where do all these variables come from?
 * They are coming from the arguments sent along with the theme('query_edit', $args) function in query-wrangler.php
 *
 * All keys in the argument array become variables in the template file
 *
 * See the following link for more details on how that works:
 * https://github.com/daggerhart/Query-Wrangler/wiki/Template-Wrangler
 */
?>
<form id="qw-edit-query-form" action='admin.php?page=query-wrangler&action=update&edit=<?php print $query_id; ?>&noheader=true' method='post'>
  <div id="qw-query-action-buttons">
    <div id="query-actions">
      <a href="admin.php?page=query-wrangler&export=<?php print $query_id; ?>">Export</a>
    </div>
    <input class='button-primary' type="submit" name="save" value="Save" />
    <input type="hidden" name="query-id" value="<?php print $query_id; ?>" />
  </div>

  <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>


  <div id="qw-query-admin-options-wrap">

    <ul>
      <!--<li><a href="#tabs-basics">Basic Settings</a></li>-->
      <li><a href="#tabs-args">Query Settings</a></li>
      <li><a href="#tabs-display">Display Settings</a></li>
      <li><a href="#tabs-page">Page Settings</a></li>
      <li><a href="#tabs-field">Fields</a></li>
      <li><a href="#tabs-filter">Filters</a></li>
      <li><a href="#tabs-contextual_filter">Contextual Filters</a></li>
      <li><a href="#tabs-sort">Sorts</a></li>
    </ul>

  <!-- basic settings 'args' -->
    <div id="tabs-args" class="qw-query-admin-tabs">
      <?php
        foreach($basics as $basic)
        {
          if ($basic['type'] == 'args')
          { ?>
            <div class="qw-setting">
              <label class="qw-label"><?php print $basic['title']; ?>:</label>
              <?php
                if(isset($basic['form']))
                { ?>
                  <div class="qw-form">
                    <?php print $basic['form']; ?>
                  </div>
                  <?php
                }
              ?>
            </div>
            <?php
          }
        }
      ?>
    </div>

  <!-- basic settings 'display' -->
    <div id="tabs-display" class="qw-query-admin-tabs">
      <?php
        foreach($basics as $basic)
        {
          if ($basic['type'] == 'display')
          { ?>
            <div class="qw-setting">
              <label class="qw-label"><?php print $basic['title']; ?>:</label>
              <?php
                if(isset($basic['form']))
                { ?>
                  <div class="qw-form">
                    <?php print $basic['form']; ?>
                  </div>
                  <?php
                }
              ?>
            </div>
            <?php
          }
        }
      ?>
    </div>

<!-- Page Settings -->
    <div id="tabs-page" class="qw-query-admin-tabs">
      <?php
        foreach($basics as $basic)
        {
          if ($basic['type'] == 'page')
          { ?>
            <div class="qw-setting">
              <?php
                // hack to prettify
                if ($basic['title'] != 'Pager')
                { ?>
                  <label class="qw-label"><?php print $basic['title']; ?>:</label>
                  <?php
                }

                if(isset($basic['form']))
                { ?>
                  <div class="qw-form">
                    <?php print $basic['form']; ?>
                  </div>
                  <?php
                }
              ?>
            </div>
            <?php
          }
        }
      ?>
    </div>

<?php
  // tab contents of each handler
  foreach($handlers as $hook_key => $handler){
    print theme('editor_tabs_handler', array('handler' => $handler));
  }

  // Add handlers
  foreach($handlers as $hook_key => $handler){
    print theme('editor_tabs_add_handler', array('handler' => $handler));
  }
?>

    <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>

  <?php print theme('preview_wrapper'); ?>
</form>