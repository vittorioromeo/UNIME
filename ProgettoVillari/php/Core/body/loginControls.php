<div class="navbar-form navbar-right">
	<div class="form-group">
		<input type="text" id="tbUser" placeholder="Username" class="form-control">
	</div>
	<div class="form-group">
		<input type="password" id="tbPass" placeholder="Password" class="form-control">
	</div>
	<a role="button" id="btnSignIn" class="btn btn-default">Sign in</a>
</div>

<script>

function signIn()
{
	trySignIn($("#tbUser").val(), $("#tbPass").val());
}

</script>

<?php

Gen::JS_PostAction('trySignIn(mUser, mPass)', 'trySignIn',
			[ 'user' => 'mUser', 'pass' => 'mPass' ],
			'reloadNavbar(); reloadPage();'
			);

Gen::JS_OnBtnClick('btnSignIn', 'signIn();');

?>