<nav class="navbar navbar-default navbar-fixed-top" role="navigation">
	<div class="container">
		<div class="navbar-header">
			<a class="navbar-brand" href="#">veeForum</a>
		</div>
		<?php

			if(Credentials::isLoggedIn() == 0)
			{
				require_once "$root/php/Core/body/loginControls.php";
			}
			else
			{
				require_once "$root/php/Core/body/profileControls.php";
			}
		?>
	</div>
</nav>