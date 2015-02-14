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
		if(trim($mName) == "''")
		{
			$mMsg = "Name not valid, empty or whitespace.";
			return false;
		}

		if($mIdParent != -1)
		{
			if(!$this->hasAnyWhere("id = $mIdParent"))
			{
				$mMsg = "Invalid parent ID: $mIdParent";
				return false;
			}

			$this->insert($mIdParent, $mName);
		}
		else
		{
			$this->insert('null', $mName);
		}

		$mMsg = "Section created successfully.";
		return true;
	}

	public function deleteWithChildren($mId, &$mMsg)
	{
		$qres = DB::query("SELECT * FROM $this->tblName 
			WHERE id_parent = $mId");

		if($qres == null)
		{
			$mMsg = "Error! " . DB::$lastError;
			return false;
		}

		while($row = $qres->fetch_assoc()) 
		{
			$this->deleteWithChildren($row["id"], $mMsg);
		}

		Tables::$section->deleteWhere("id = $mId");
		$mMsg = "Success.";
		return true;
	}	

	public function getHierarchyStr($mParent = 'null', $mIndent = "")
	{
		$res = "";
		$where = "";
		if($mParent == 'null') 
		{
			$where = "id_parent IS NULL";
		}
		else
		{
			$where = "id_parent = $mParent";
		}

		$qres = DB::query("SELECT * FROM $this->tblName WHERE $where");

		if($qres == null)
		{
			return "Error getting section hierarchy. " . DB::$lastError;
		}

		while($row = $qres->fetch_assoc()) 
		{
			$id = $row['id'];
			$name = $row['name'];

			$res .= $mIndent . "($id) $name\n";
			$res .= $this->getHierarchyStr($id, $mIndent . "--->");
		}

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
Tables::$user->setInsertFields('id_group', 'username', 'password_hash', 'email', 'registration_date', 'firstname', 'lastname', 'birthdate');

// Tables::$group->insert('null', DB::v("agdssad"), DB::v("TTTTT"));



?>