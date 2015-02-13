<?php

$root = realpath($_SERVER["DOCUMENT_ROOT"]) . "/PV";
include "$root/php/base.php";

$idParent = $_POST["idParent"];
$name = $_POST["name"];
$msg = "";

Tables::$section->mkSection($idParent, $name, $msg);

echo $idParent;
echo $name;
echo $msg;

?>