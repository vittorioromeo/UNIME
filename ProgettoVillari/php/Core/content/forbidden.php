<?php
	$root = realpath($_SERVER["DOCUMENT_ROOT"]) . "/PV";
	require_once("$root/php/Lib/lib.php");
?>

<br/>

<?php
	if(Credentials::isLoggedIn())
	{
		print('<strong>Permissions not valid to view this page.</strong>');
	}
	else
	{
		print('<strong>User not logged in.</strong>');
	}
?>