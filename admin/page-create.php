<div>
  <p>
    Choose the name and the display types of your query.
  </p>
</div>

<div id="qw-create">
  <form action='admin.php?page=query-wrangler&action=create&noheader=true' method='post'>
    <div class="qw-setting">
      <label class="qw-label">Query Name:</label>
      <input class="qw-create-input" type="text" name="qw-name" value="" />
      <p class="description">Query name is a way for you, the admin, to identify the query easily.</p>
    </div>

    <div class="qw-setting">
      <h4>Display Types</h4>
      <p class="description">How should the query bew presented to users?</p>
      <?php
        $display_types = qw_all_display_types();
        foreach ($display_types as $key => $display_type)
        { ?>
          <div class="clear-left">
            <label class="qw-label"><?php print $display_type['title']; ?></label>
            <input type="checkbox"
                   name="qw-display[types][<?php print $key; ?>]"
                   value="<?php print $key; ?>" />

            <p class="description clear-left"><?php print $display_type['description']; ?></p>
          </div>
          <?php
        }
      ?>
    </div>

    <div class="qw-setting">
      <label class="qw-label">Data:</label>
      <select name="qw-data-wizard" id="qw-data-wizard">
      <?php
        foreach($wizards as $name => $wizard)
        { ?>
          <option value="<?php print $name; ?>"><?php print $wizard['title']; ?></option>
          <?php
        }
      ?>
      </select>
      <p class="description">Choose the starting data for your query.</p>
      <div id="wizard-descriptions">
        <?php
          foreach ($wizards as $name => $wizard)
          { ?>
            <p id="wizard-<?php print $name; ?>" class="description"><?php print $wizard['description']; ?></p>
            <?php
          }
        ?>
      </div>
    </div>

    <div class="qw-create">
      <input type="submit" value="Create" class="button-primary" />
    </div>
  </form>
</div>

<div id="qw-create-description">
  <div>
    <h3>Widgets</h3>
    <p>
      The Query Wrangler comes with a reusable Wordpress Widget that an be places in sidebars.
      When you create a query of the this type, that query becomes selectable in the Widget settings.
    </p>
  </div>
  <div>
    <h3>Pages</h3>
    <p>
      When you create a Page Query, you give that query a path (URI) to display on.
      After creating the query, you can visit that URI on your website to view the results.
      This is a great way to create new, complex pages on your Wordpress site.
      <br /><br />
      <strong><em>Pages do not work with the Default permalink structure found <a href="<?php print get_bloginfo('wpurl'); ?>/wp-admin/options-permalink.php">here</a>.</em></strong>
    </p>
  </div>
</div>