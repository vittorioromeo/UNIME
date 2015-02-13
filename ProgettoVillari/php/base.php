<?php

$root = realpath($_SERVER["DOCUMENT_ROOT"]) . "/PV";

ini_set('display_errors',1);
ini_set('display_startup_errors',1);
error_reporting(-1);

require_once "$root/php/Global/global.php";
Session::init();

?>