<?php

$root = realpath($_SERVER["DOCUMENT_ROOT"]) . "/PV";

ini_set('display_errors',1);
ini_set('display_startup_errors',1);
error_reporting(-1);
date_default_timezone_set('UTC');

require_once("$root/php/Lib/Impl/session.php");
require_once("$root/php/Lib/Impl/debug.php");
require_once("$root/php/Lib/Impl/db.php");
require_once("$root/php/Lib/Impl/utils.php");
require_once("$root/php/Lib/Impl/gen.php");
require_once("$root/php/Lib/Impl/credentials.php");
require_once("$root/php/Lib/Impl/tbl.php");
require_once("$root/php/Lib/Impl/tblImpl.php");

Session::init();

?>