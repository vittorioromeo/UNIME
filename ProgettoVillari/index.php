<?php

ini_set('display_errors',1);
ini_set('display_startup_errors',1);
error_reporting(-1);

include "php/Global/global.php";

Session::init();

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

include "php/Core/head.php";
include "php/Core/body.php";



$res = Tables::$group->getAllRows();

$res->data_seek(0);
while ($row = $res->fetch_assoc()) 
{
    Debug::lo(" id = " . $row["id"]);
}

Debug::echoLo();

?>

</html>