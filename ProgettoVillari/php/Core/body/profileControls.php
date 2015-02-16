<ul class="nav navbar-nav">
	<?php
		if(Credentials::hasCUPrivilege(Privs::$superAdmin))
		{
			print('<li><a href="#">Administration</a></li>');
		}
	?>   
	<li><a href="#">Sections</a></li>
</ul>

<ul class="nav navbar-nav navbar-right">
	<li>
		<a href="#">
			Logged in as: 
			<strong> <?php print(Credentials::getCURow()['username']); ?> </strong>
		</a>
	</li>   
	<li>
		<div class="navbar-form">
			<?php Gen::LinkBtn('btnSignOut', '', 'Sign out'); ?>			
		</div>
	</li>   		
</ul>

<?php

Gen::JS_PostAction('trySignOut()', 'trySignOut',
			array(),
			'reloadNavbar(); reloadPage();'
			);

Gen::JS_OnBtnClick('btnSignOut', 'trySignOut(); ');

?>