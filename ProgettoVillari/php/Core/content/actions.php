<?php

$root = realpath($_SERVER["DOCUMENT_ROOT"]) . "/PV";
require_once("$root/php/Lib/lib.php");

$action = $_POST["action"];
if(!isset($action)) exit(1);

class ActionUtils
{
	public static function printQuerySuccess($mRes)
	{
		print($mRes ? "Success." : DB::$lastError);
	}
}

class Actions
{
	public static function setDebugEnabled()
	{
		Debug::setEnabled($_POST["enabled"] == "true");		
	}

	public static function refreshDebugLo()
	{
		Debug::echoLo();
	}


	
	public static function trySignIn()
	{
		print(Credentials::tryLogin($_POST["user"], $_POST["pass"]));
	}

	public static function trySignOut()
	{
		print(Credentials::tryLogout());
	}		

	public static function getCurrentPage()
	{
		if(Credentials::isLoggedIn() && Credentials::canCUViewCurrentPage())
		{
			if(Credentials::hasCUPrivilege(Privs::$superAdmin))
			{				
				print(Pages::getCurrent()->getURL());
				return;
			}
		}
		
		print('php/Core/content/forbidden.php');		
	}



	public static function scAdd()
	{
		$idParent = $_POST["idParent"];
		if($idParent == -1) $idParent = 'null';

		$res = TBS::$section->insertValues($idParent, $_POST["name"]);
		ActionUtils::printQuerySuccess($res);
	}
	
	public static function changeCurrentPage()
	{
		$idpage = $_POST["idpage"];
		Session::set(SK::$pageID, $idpage);
	}

	public static function scDel()
	{
		$res = TBS::$section->deleteByID($_POST["id"]);
		ActionUtils::printQuerySuccess($res);
	}

	public static function scDelRecursive()
	{		
		$res = TBS::$section->deleteRecursiveByID($_POST["id"]);
		ActionUtils::printQuerySuccess($res);
	}

	public static function getSectionOptions()
	{
		$nullRow = $_POST["nullRow"];

		if($nullRow == "true")
		{
			print('<option value="-1">NULL</option>');
		}

		foreach(TBS::$section->getAll() as $x)
		{
			print("<option value=".$x["id"].">(ID: ".$x["id"].") ".$x["name"]."</option>");
		}
	}

	public static function getSectionHierarchyStr()
	{
		print(nl2br(TBS::$section->getHierarchyStr()));
	}





	public static function grAdd()
	{
		$idParent = $_POST["idParent"];
		$name = $_POST["name"];
		$privileges = $_POST["privileges"];
		$msg = "";

		$pset = new PrivSet();
		if($privileges) foreach($privileges as $x) $pset->add($x);

		TBS::$group->mkGroup($idParent, DB::v($name), $pset, $msg);

		print($msg);	
	}

	public static function grDel()
	{
		$res = TBS::$group->deleteByID($_POST["id"]);
		print($res ? "Success." : DB::$lastError);
	}

	public static function grDelRecursive()
	{
		$res = TBS::$group->deleteRecursiveByID($_POST["id"]);
		print($res ? "Success." : DB::$lastError);
	}

	public static function getGroupOptions()
	{
		$nullRow = $_POST["nullRow"];

		if($nullRow == "true")
		{
			print('<option value="-1">NULL</option>');
		}

		foreach(TBS::$group->getAll() as $x)
		{
			print("<option value=".$x["id"].">(ID: ".$x["id"].") ".$x["name"]."</option>");
		}
	}

	public static function getGroupHierarchyStr()
	{
		print(nl2br(TBS::$group->getHierarchyStr()));
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

		$res = "";

		if($id == -1)
		{
			$res = TBS::$user->insertValues($groupId, $username, $passwordHash, $email, $registrationDate, $firstname, $lastname, $birthname);			
		}
		else
		{
			$res = TBS::$user->updateByID($id, 
			[			
				'id_group' => $groupId,
				'username' => $username,
				'email' => $email,
				'registration_date' => $registrationDate,
				'firstname' => $firstname,
				'lastname' => $lastname,
				'birth_date' => $birthdate
			]);
		}

		print($res ? "Success." : DB::$lastError);
	}

	public static function usDel()
	{
		$id = $_POST["id"];

		TBS::$user->deleteByID($id);

		print("Success.");
	}

	public static function usGetData()
	{
		$id = $_POST["id"];
		$r = TBS::$user->getFirstWhere('id = '.DB::v($id));
		$res = 
		[
			'username' => $r['username'],
			'email' => $r['email'],
			'groupid' => $r['id_group'],
			'firstname' => $r['firstname'],
			'lastname' => $r['lastname'],
			'registrationdate' => $r['registration_date'],
			'birthdate' => $r['birth_date']
		];

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

			foreach(TBS::$user->getAll() as $x)
			{
				print('<tr>');

				$userId = $x['id'];
				$groupId = $x['id_group'];
				$groupName = TBS::$group->getFirstWhere('id = '.DB::v($groupId))['name'];
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

	public static function getGSPData()
	{
		$idGroup = $_POST["idgroup"];
		$idSection = $_POST["idsection"];
		$where = 'id_group = '.DB::v($idGroup).' AND id_section = '.DB::v($idSection);

		if(!TBS::$gsperms->hasAnyWhere($where))
		{
			TBS::$gsperms->insertValues($idGroup, $idSection, false, false, false, false, false, false);
		}

		$r = TBS::$gsperms->getFirstWhere($where);
		$res = 
		[
			'cpost' => $r['can_post'],
			'cview' => $r['can_view'],
			'ccreatethread' => $r['can_create_thread'],
			'cdeletepost' => $r['can_delete_post'],
			'cdeletethread' => $r['can_delete_thread'],
			'cdeletesection' => $r['can_delete_section']
		];

		$tj = json_encode($res);
		print($tj);
	}

	public static function setGSPData()
	{
		$idGroup = $_POST["idgroup"];
		$idSection = $_POST["idsection"];;

		$cpost = $_POST["cpost"];
		$cview = $_POST["cview"];
		$ccreatethread = $_POST["ccreatethread"];
		$cdeletepost = $_POST["cdeletepost"];
		$cdeletethread = $_POST["cdeletethread"];
		$cdeletesection = $_POST["cdeletesection"];

		$where = 'id_group = '.DB::v($idGroup).' AND id_section = '.DB::v($idSection);
		$r = TBS::$gsperms->getFirstWhere($where);
		$id = $r['id'];

		print(TBS::$gsperms->updateByID($id,
		[
			'can_post' => $cpost,
			'can_view' => $cview,
			'can_create_thread' => $ccreatethread,
			'can_delete_post' => $cdeletepost,
			'can_delete_thread' => $cdeletethread,
			'can_delete_section' => $cdeletesection,
		]));
	}
}

Actions::$action();

?>
		