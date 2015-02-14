<?php

$root = realpath($_SERVER["DOCUMENT_ROOT"]) . "/PV";
require_once("$root/php/Lib/lib.php");

$action = $_POST["action"];
if(!isset($action))
{
	exit(1);
}

class Actions
{
	public static function setDebugEnabled()
	{
		$enabled = $_POST["enabled"];

		if($enabled == "true")
		{
			Debug::enable();
		}
		else
		{
			Debug::disable();
		}
	}
	
	public static function createSection()
	{
		$idParent = $_POST["idParent"];
		$name = $_POST["name"];
		$msg = "";

		Tables::$section->mkSection($idParent, DB::v($name), $msg);

		print($msg);
	}
	
	public static function getSectionOptions()
	{
		$nullRow = $_POST["nullRow"];

		if($nullRow == "true")
		{
			print('<option value="-1">NULL</option>');
		}

		foreach(Tables::$section->getAllRows() as $x)
		{
			print("<option value=".$x["id"].">(ID: ".$x["id"].") ".$x["name"]."</option>");
		}
	}

	public static function refreshDebugLo()
	{
		Debug::echoLo();
	}

	public static function deleteSection()
	{
		$id = $_POST["id"];
		$res = Tables::$section->deleteWhere("id = $id");

		print(($res == null) ? DB::$lastError : "Success.");
	}

	public static function deleteSectionWithChildren()
	{
		$id = $_POST["id"];
		$msg = "";
		
		Tables::$section->deleteWithChildren($id, $msg);

		print($msg);
	}

	public static function getSectionHierarchyStr()
	{
		print(nl2br(Tables::$section->getHierarchyStr()));
	}
};

Actions::$action();

?>