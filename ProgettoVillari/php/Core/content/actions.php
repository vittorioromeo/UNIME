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

	public static function refreshDebugLo()
	{
		Debug::echoLo();
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
		$msg = "";
		print(nl2br(Tables::$section->getHierarchyStr($msg)));
	}





	public static function createGroup()
	{
		$idParent = $_POST["idParent"];
		$name = $_POST["name"];
		$privileges = $_POST["privileges"];
		$msg = "";

		Tables::$group->mkGroup($idParent, DB::v($name), $privileges, $msg);

		print($msg);
	}
	
	public static function getGroupOptions()
	{
		$nullRow = $_POST["nullRow"];

		if($nullRow == "true")
		{
			print('<option value="-1">NULL</option>');
		}

		foreach(Tables::$group->getAllRows() as $x)
		{
			print("<option value=".$x["id"].">(ID: ".$x["id"].") ".$x["name"]."</option>");
		}
	}

	public static function deleteGroup()
	{
		$id = $_POST["id"];
		$res = Tables::$group->deleteWhere("id = $id");

		print(($res == null) ? DB::$lastError : "Success.");
	}

	public static function deleteGroupWithChildren()
	{
		$id = $_POST["id"];
		$msg = "";
		
		Tables::$group->deleteWithChildren($id, $msg);

		print($msg);
	}

	public static function getGroupHierarchyStr()
	{
		$msg = "";
		print(nl2br(Tables::$group->getHierarchyStr($msg)));
	}




	public static function getTblUsers()
	{
		print('
		<thead>
			<tr>
				<th>
					<a class="btn btn-default btn-xs" href="#" role="button" id="btnUsAdd">
						<span class="glyphicon glyphicon-plus" aria-hidden="true"></span>				
					</a>
				</th>
				<th>Username</th>
				<th>Email</th>
				<th>Group</th>
				<th>First name</th>
				<th>Last name</th>
				<th>Registration date</th>
				<th>Birth date</th>
			</tr>
		</thead>
		');

		print('<tbody>');

		print('<tr>');

		// Actions
		print('
		<td>
			<a class="btn btn-default btn-xs" href="#" role="button" id="btnUserDel" data-id="0">
				<span class="glyphicon glyphicon-remove" aria-hidden="true"></span>				
			</a>
		</td>
		');

		print('<td>username_0</td>');
		print('<td>email_0</td>');
		print('<td>group_0</td>');
		print('<td>firstname_0</td>');
		print('<td>lastname_0</td>');
		print('<td>regdate_0</td>');
		print('<td>birthdate_0</td>');

		print('</tr>');

		print('</tbody>');
	}
};

Actions::$action();

?>