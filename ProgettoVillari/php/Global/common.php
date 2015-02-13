<?php

class TblUser extends Tbl
{
	
}

class TblGroup extends Tbl
{
	
}

class TblSection extends Tbl
{
	public function mkSection($mIdParent, $mName, &$mMsg)
	{
		if(!$this->hasAnyWhere("id = $mIdParent"))
		{
			$mMsg = "Invalid parent ID: $mIdParent";
			return false;
		}

		if(trim($mName) == "")
		{
			$mMsg = "Name not valid, empty or whitespace.";
			return false;
		}

		$this->insert($mIdParent, $mName);
		$mMsg = "Section created successfully.";
		return true;
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
Tables::$user->setInsertFields('id_group', 'username', 'password_hash', 'email', 'registration_date', 'firstname', 'lastname', 'birthdate');

// Tables::$group->insert('null', DB::v("agdssad"), DB::v("TTTTT"));



?>