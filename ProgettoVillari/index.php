<?php

$root = realpath($_SERVER["DOCUMENT_ROOT"]) . "/PV";
require_once "$root/php/Lib/lib.php";

?>

<!doctype html>
<!--[if lt IE 7]>      <html class="no-js lt-ie9 lt-ie8 lt-ie7" lang=""> <![endif]-->
<!--[if IE 7]>         <html class="no-js lt-ie9 lt-ie8" lang=""> <![endif]-->
<!--[if IE 8]>         <html class="no-js lt-ie9" lang=""> <![endif]-->
<!--[if gt IE 8]><!--> <html class="no-js" lang=""> <!--<![endif]-->

<?php

if(isset($_POST['user']))
{
	Session::set(SKeys::$loggedIn, true);
}
else
{
}

require_once "$root/php/Core/head.php";
require_once "$root/php/Core/body.php";



$res = Tables::$group->getAllRows();

$res->data_seek(0);
while ($row = $res->fetch_assoc()) 
{
   // Debug::lo(" id = " . $row["id"]);
}

	Debug::loLn();
	Debug::lo("<br/>Session ID: <br/>" . session_id()); 
	Debug::lo("<br/>Session data: <br/>" . print_r($_SESSION, true));
?>

<div id="debugLo">
	<?php Debug::echoLo(); ?>
</div>

</html>