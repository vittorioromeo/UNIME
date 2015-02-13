<body>
<!--[if lt IE 8]>
		<p class="browserupgrade">
			You are using an <strong>outdated</strong> browser. 
			Please <a href="http://browsehappy.com/">upgrade your browser</a> to improve your experience.
		</p>
<![endif]-->

<?php
	$root = realpath($_SERVER["DOCUMENT_ROOT"]) . "/PV";

	include "$root/php/Core/body/navbar.php";
	// include "body/jumbotron.php";
	include "$root/php/Core/content/adminPanel.php";
	include "$root/php/Core/body/scripts.php";
?>

</body>