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


	
	public static function trySignIn()
	{
		$user = $_POST["user"];
		$pass = $_POST["pass"];
		
		$res = Credentials::tryLogin($user, $pass);
		print($res);
	}

	public static function trySignOut()
	{
		print(Credentials::tryLogout());
	}		

	public static function getCurrentPage()
	{
		if(Credentials::isLoggedIn())
		{
			if(Credentials::hasCUPrivilege(Privs::$superAdmin))
			{
				print('php/Core/content/adminPanel.php');
				return;
			}
		}

		print('php/Core/content/forbidden.php');
	}



	public static function scAdd()
	{
		$idParent = $_POST["idParent"];
		$name = $_POST["name"];
		$msg = "";

		Tables::$section->mkSection($idParent, DB::v($name), $msg);

		print($msg);
	}
	
	public static function scDel()
	{
		$id = $_POST["id"];
		$res = Tables::$section->deleteWhere("id = $id");

		print(($res == null) ? DB::$lastError : "Success.");
	}

	public static function scDelRecursive()
	{
		$id = $_POST["id"];
		$msg = "";
		
		Tables::$section->deleteWithChildren($id, $msg);

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

	public static function getSectionHierarchyStr()
	{
		$msg = "";
		print(nl2br(Tables::$section->getHierarchyStr($msg)));
	}





	public static function grAdd()
	{
		$idParent = $_POST["idParent"];
		$name = $_POST["name"];
		$privileges = $_POST["privileges"];
		$msg = "";

		$pset = new PrivSet();
		if($privileges) foreach($privileges as $x) $pset->add($x);

		Tables::$group->mkGroup($idParent, DB::v($name), $pset, $msg);

		print($msg);	
	}

	public static function grDel()
	{
		$id = $_POST["id"];
		$res = Tables::$group->deleteWhere("id = $id");

		print(($res == null) ? DB::$lastError : "Success.");
	}

	public static function grDelRecursive()
	{
		$id = $_POST["id"];
		$msg = "";
		
		Tables::$group->deleteWithChildren($id, $msg);

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

	public static function getGroupHierarchyStr()
	{
		$msg = "";
		print(nl2br(Tables::$group->getHierarchyStr($msg)));
	}

	public static function usAdd()
	{
		$id = $_POST['id'];

		$groupId = $_POST['groupId'];
		$username = $_POST['username'];
		$passwordHash = Utils::getPwdHash($_POST['password']);
		$email = $_POST['email'];
		$registrationDate = date('Y-m-d');
		$firstname = $_POST['firstname'];
		$lastname = $_POST['lastname'];
		$birthdate = $_POST['birthdate'];

		if($id == -1)
		{
			return Tables::$user->insert
			(
				DB::v($groupId), 
				DB::v($username), 
				DB::v($passwordHash), 
				DB::v($email), 
				DB::v($registrationDate), 
				DB::v($firstname),
				DB::v($lastname), 
				DB::v($birthdate)
			);
		}
		else
		{
			return DB::query('UPDATE tbl_user 
				SET 
					id_group = '.DB::v($groupId).',
					username = '.DB::v($username).',
					email = '.DB::v($email).',
					registration_date = '.DB::v($registrationDate).',
					firstname = '.DB::v($firstname).',
					lastname = '.DB::v($lastname).',
					birth_date = '.DB::v($birthdate).'
				WHERE 
					id = '.DB::v($id)
				);
		}
	}

	public static function usDel()
	{
		$id = $_POST["id"];

		Tables::$user->deleteWhere('id = '.DB::v($id));

		print("Success.");
	}

	public static function usGetData()
	{
		$id = $_POST["id"];
		$r = Tables::$user->firstRowWhere('id = '.DB::v($id));
		$res = array
		(
			'username' => $r['username'],
			'email' => $r['email'],
			'groupid' => $r['id_group'],
			'firstname' => $r['firstname'],
			'lastname' => $r['lastname'],
			'registrationdate' => $r['registration_date'],
			'birthdate' => $r['birth_date']
		);

		$tj = json_encode($res);
		print($tj);
	}

	public static function getTblUsers()
	{
		print('
		<thead>
			<tr>
				<th>');
					Gen::LinkBtn('btnUsAdd', 'glyphicon-plus', '', 'btn-xs');
		print('</th>
				<th>ID</th>
				<th>Username</th>
				<th>Email</th>
				<th>Group</th>
				<th>First name</th>
				<th>Last name</th>
				<th>Registration date</th>
				<th>Birth date</th>
			</tr>
		</thead>');

		print('<tbody>');

			foreach(Tables::$user->getAllRows() as $x)
			{
				print('<tr>');

				$userId = $x['id'];
				$groupId = $x['id_group'];
				$groupName = Tables::$group->firstRowWhere('id = '.DB::v($groupId))['name'];
				$btnActionsId = 'btnUsActions_' . $userId;
				$btnEditId = 'btnUsEdit_' . $userId;

				print('<td>');
					Gen::LinkBtn($btnActionsId, 'glyphicon-asterisk', '', 'btn-xs');
					Gen::JS_OnBtnClick($btnActionsId, 'showUsActionsModal('.$userId.');');
					
					Gen::LinkBtn($btnEditId, 'glyphicon-pencil', '', 'btn-xs');
					Gen::JS_OnBtnClick($btnEditId, 'showUsEditModal('.$userId.');');
				print('</td>');

				print('<td>'.$userId.'</td>');
				print('<td>'.$x['username'].'</td>');
				print('<td>'.$x['email'].'</td>');
				print('<td>'.$groupName.'</td>');
				print('<td>'.$x['firstname'].'</td>');
				print('<td>'.$x['lastname'].'</td>');
				print('<td>'.$x['registration_date'].'</td>');
				print('<td>'.$x['birth_date'].'</td>');
			
				print('</tr>');
			}
				
		print('</tbody>');
	}
}

Actions::$action();

?>
		