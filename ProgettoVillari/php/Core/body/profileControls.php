
<form method="post" class="navbar-form navbar-right" role="form">
	Logged in as: <?php echo "TODO"; ?>



	<div class="form-group">
		<label for="btnLogout" text="Logged in as: <?php echo Session::get(SKeys::$userID); ?>" />
	</div>
	<button id="btnLogout" name="btnLogout" type="submit" class="btn btn-success">Logout</button>
</form>
 