
<form method="post" class="navbar-form navbar-right" role="form">
	Logged in as: 
	<strong>
	<?php 
		print(Credentials::getCURow()['username']); 
	?>
	</strong>



	<a id="btnSignOut" role="button" type="submit" class="btn btn-success">Sign out</a>
</form>

<?php

Gen::JS_PostAction('trySignOut()', 'trySignOut',
			array(),
			'reloadNavbar(); reloadPage();'
			);

Gen::JS_OnBtnClick('btnSignOut', 'trySignOut(); ');

?>