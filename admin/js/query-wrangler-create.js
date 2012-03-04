function qw_toggle_wizard_description(){
  // hide all
  jQuery('#wizard-descriptions p').hide();

  // get value and show description
  var val = jQuery('#qw-data-wizard').val();
  console.log(val);
  jQuery('p#wizard-'+val).show();
}

jQuery(document).ready(function(){
  qw_toggle_wizard_description();
  jQuery('#qw-data-wizard').change(function(){
    qw_toggle_wizard_description();
  });
});