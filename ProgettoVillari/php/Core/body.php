<body>
<!--[if lt IE 8]>
		<p class="browserupgrade">
			You are using an <strong>outdated</strong> browser. 
			Please <a href="http://browsehappy.com/">upgrade your browser</a> to improve your experience.
		</p>
<![endif]-->

<?php
	//$root = realpath($_SERVER["DOCUMENT_ROOT"]) . "/PV";

	require_once "$root/php/Core/body/navbar.php";
	// include "body/jumbotron.php";

	require_once "$root/php/Core/body/modalInfo.php";

	print('<div class="container">');
	require_once "$root/php/Core/content/adminPanel.php";
	require_once "$root/php/Core/body/footer.php";
	print('</div>');

	require_once "$root/php/Core/body/scripts.php";
?>

</body>