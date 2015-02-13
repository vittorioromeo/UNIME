<?php

$root = realpath($_SERVER["DOCUMENT_ROOT"]) . "/PV";
require_once("$root/php/Lib/lib.php");

$idParent = $_POST["idParent"];
$name = $_POST["name"];
$msg = "";

Tables::$section->mkSection($idParent, DB::v($name), $msg);

print($msg);

?>