<nav class="navbar navbar-inverse navbar-fixed-top" role="navigation">
	<div class="container">
		<div class="navbar-header">
			<a class="navbar-brand" href="#">veeForum</a>
		</div>
		<?php
			if(Credentials::isLoggedIn() == 1)
			{
				echo "diosepre";
				include "loginControls.php";
			}
			else
			{
				include "profileControls.php";
			}
		?>
	</div>
</nav>