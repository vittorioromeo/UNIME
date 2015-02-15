<form method="post" class="navbar-form navbar-right" role="form">
	<div class="form-group">
		<input type="text" id="tbUser" placeholder="Username" class="form-control">
	</div>
	<div class="form-group">
		<input type="password" id="tbPass" placeholder="Password" class="form-control">
	</div>
	<a role="button" id="btnSignIn" class="btn btn-success">Sign in</a>
</form>

<script>

function signIn()
{
	trySignIn($("#tbUser").val(), $("#tbPass").val());
}

</script>

<?php

Gen::JS_PostAction('trySignIn(mUser, mPass)', 'trySignIn',
			array( 'user' => 'mUser', 'pass' => 'mPass' ),
			' reloadNavbar(); reloadPage();'
			);

Gen::JS_OnBtnClick('btnSignIn', 'signIn();');

?>