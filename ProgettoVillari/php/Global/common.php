<?php

class TblUser extends Tbl
{
	
}

class TblGroup extends Tbl
{
	
}

class Tables
{
	public static $group;
	public static $user;
}

Tables::$group = new TblGroup('tbl_group');
Tables::$group->setInsertFields('id_parent', 'name', 'privileges');

Tables::$user = new TblUser('tbl_user');
Tables::$user->setInsertFields('id_group', 'username', 'password_hash', 'email', 'registration_date', 'firstname', 'lastname', 'birthdate');

Tables::$group->insert('null', DB::v("dio scollato"), DB::v("TTTTT"));



?>