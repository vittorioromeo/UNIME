
<form method="post" class="navbar-form navbar-right" role="form">
	Logged in as: <?php echo "TODO"; ?>



	<div class="form-group">
		<label for="btnLogout" text="Logged in as: <?php echo Session::get(SKeys::$userID); ?>" />
	</div>
	<a id="btnSignOut" role="button" type="submit" class="btn btn-success">Sign out</a>
</form>

<?php

Gen::JS_PostAction('trySignOut()', 'trySignOut',
			array(),
			'alert(mOut); reloadNavbar();'
			);

Gen::JS_OnBtnClick('btnSignOut', 'trySignOut();');

?>