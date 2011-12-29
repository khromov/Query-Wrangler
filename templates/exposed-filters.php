<div id="qw-exposed-filters">
<form method="get" action="">
<?php foreach($filters as $filter): ?>
<select name="<?php echo $filter['name']; ?>">
	<option value="">Select <?php echo $filter['select_label']; ?></option>
	<?php foreach($filter['options'] as $option => $label): ?>
	<option value="<?php echo $option; ?>" <?php if(@$_REQUEST[$filter['name']] == $option) { echo 'selected'; } ?>><?php echo $label; ?></option>
	<?php endforeach; ?>
</select>
<?php endforeach; ?>
<input type="submit" value="Filter" name="" />
</form>
</div>