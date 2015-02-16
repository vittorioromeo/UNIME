<?php

class TblUser extends Tbl
{
	public function mkUser()
	{

	}

	public function findByCredentials($mUser, $mPass)
	{
		$hash = Utils::getPwdHash($mPass);
		return $this->getFirstWhere('username = '.DB::v($mUser).' AND password_hash = '.DB::v($hash));
	}
}

class TblGroup extends Tbl
{
	public function mkGroup($mIDParent, $mName, $mPrivs, &$mMsg)
	{
		if(!Utils::checkEmptyStr($mName, $mMsg)) return false;

		$parentId = Utils::getInsertParent($this, $mIDParent, $mMsg);
		if(!$parentId) return false;

		$this->insert($parentId, $mName, DB::v($mPrivs->toStr()));
		$mMsg = "Group created successfully.";
		return true;
	}

	public function getHierarchyStr()
	{	
		$res = "";

		$this->forChildren(function(&$mRow, $mDepth) use (&$res)
		{
			$indent = str_repeat("--->", $mDepth);

			$id = $mRow['id'];
			$name = $mRow['name'];
			$privileges = $mRow['privileges'];

			$res .= $indent . "($id) $name [$privileges]\n";
		});

		return $res;
	}
}

class TblSection extends Tbl
{
	public function mkSection($mIDParent, $mName, &$mMsg)
	{
		if(!Utils::checkEmptyStr($mName, $mMsg)) return false;

		$parentId = Utils::getInsertParent($this, $mIDParent, $mMsg);
		if(!$parentId) return false;

		$res = $this->insert($parentId, $mName);
		$mMsg = "Section created successfully.";
		
		return !$res ? false : true; 
	}

	public function getHierarchyStr()
	{	
		$res = "";

		$this->forChildren(function(&$mRow, $mDepth) use (&$res)
		{
			$indent = str_repeat("--->", $mDepth);

			$id = $mRow['id'];
			$name = $mRow['name'];

			$res .= $indent . "($id) $name\n";
		});

		return $res;
	}
}

class TblGroupSectionPermission extends Tbl
{
	public function mkGSPerm($mIDGroup, $mIDSection, $mCView, $mCPost, $mCCreateThread, $mCDeletePost, $mCDeleteThread, $mCDeleteSection)
	{
		$res = $this->insert
		(
			DB::v($mIDGroup), 
			DB::v($mIDSection), 
			DB::v($mCView), 
			DB::v($mCPost), 
			DB::v($mCCreateThread), 
			DB::v($mCDeletePost), 
			DB::v($mCDeleteThread), 
			DB::v($mCDeleteSection)
		);

		return !$res ? false : true; 
	}
};

class TBS
{
	public static $section;
	public static $group;
	public static $user;
	public static $gsperms;
}

TBS::$section = new TblSection('tbl_section');
TBS::$section->setInsertFields('id_parent', 'name');

TBS::$group = new TblGroup('tbl_group');
TBS::$group->setInsertFields('id_parent', 'name', 'privileges');

TBS::$user = new TblUser('tbl_user');
TBS::$user->setInsertFields('id_group', 'username', 'password_hash', 'email', 'registration_date', 'firstname', 'lastname', 'birth_date');

TBS::$gsperms = new TblGroupSectionPermission('tbl_group_section_permission');
TBS::$gsperms->setInsertFields('id_group', 'id_section', 'can_view', 'can_post', 'can_create_thread', 'can_delete_post', 'can_delete_thread', 'can_delete_section');

?>