<?php

class TblUser extends Tbl
{
	public function mkUser()
	{

	}
}

class TblGroup extends Tbl
{
	public function mkGroup($mIdParent, $mName, $mPrivileges, &$mMsg)
	{
		if(!Utils::checkEmptyStr($mName, $mMsg)) return false;

		$parentId = Utils::getInsertParent($this, $mIdParent, $mMsg);
		if(!$parentId) return false;

		$privileges = Privileges::arrayToStr($mPrivileges);

		$this->insert($parentId, $mName, DB::v($privileges));
		$mMsg = "Group created successfully.";
		return true;
	}

	public function getHierarchyStr(&$mMsg)
	{	
		$res = "";

		$this->forHierarchy(function($mRow, $mDepth) use (&$res)
		{
			$indent = str_repeat("--->", $mDepth);

			$id = $mRow['id'];
			$name = $mRow['name'];
			$privileges = $mRow['privileges'];

			$res .= $indent . "($id) $name [$privileges]\n";

			return $id;
		}, $mMsg);

		return $res;
	}
}

class TblSection extends Tbl
{
	public function mkSection($mIdParent, $mName, &$mMsg)
	{
		if(!Utils::checkEmptyStr($mName, $mMsg)) return false;

		$parentId = Utils::getInsertParent($this, $mIdParent, $mMsg);
		if(!$parentId) return false;

		$this->insert($parentId, $mName);
		$mMsg = "Section created successfully.";
		return true;
	}

	public function getHierarchyStr(&$mMsg)
	{	
		$res = "";

		$this->forHierarchy(function($mRow, $mDepth) use (&$res)
		{
			$indent = str_repeat("--->", $mDepth);

			$id = $mRow['id'];
			$name = $mRow['name'];

			$res .= $indent . "($id) $name\n";

			return $id;
		}, $mMsg);

		return $res;
	}
}

class Tables
{
	public static $section;
	public static $group;
	public static $user;
}

Tables::$section = new TblSection('tbl_section');
Tables::$section->setInsertFields('id_parent', 'name');

Tables::$group = new TblGroup('tbl_group');
Tables::$group->setInsertFields('id_parent', 'name', 'privileges');

Tables::$user = new TblUser('tbl_user');
Tables::$user->setInsertFields('id_group', 'username', 'password_hash', 'email', 'registration_date', 'firstname', 'lastname', 'birth_date');

// Tables::$group->insert('null', DB::v("agdssad"), DB::v("TTTTT"));



?>