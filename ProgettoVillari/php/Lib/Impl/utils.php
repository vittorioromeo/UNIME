<?php

class PrivilegesSet
{
	public $superAdmin = false;
}

class Privileges
{
	public static $count = 1;

	public static function fromStr($mX)
	{
		$res = new PrivilegesSet();

		$res->superAdmin = $mX[0] == 'T';
	
		return $res;
	}
	public static function toStr($mX)
	{
		$res = "";

		$res .= ($mX->superAdmin) ? "T" : "F";

		return $res;
	}
	public static function arrayToStr($mX)
	{
		$res = "";

		$res .= (in_array(0, $mX)) ? "T" : "F"; 

		return $res;
	}
}

class Utils
{
	public static function GetCSL($mArray)
	{
		$res = "";

		for ($x = 0; $x < count($mArray); ++$x) 
		{	
			$res .= $mArray[$x];
			if($x < count($mArray) - 1) $res .= ", ";
		}

		return $res;
	}

	public static function GetPwdHash($mX)
	{
		return md5($mX);
	}

	public static function checkEmptyStr($mX, &$mMsg)
	{
		if(trim($mX) == "''")
		{
			$mMsg = "String not valid, empty or whitespace.";
			return false;
		}

		return true;
	}

	public static function getInsertParent(&$mTbl, $mIdParent, &$mMsg)
	{
		if($mIdParent != -1)
		{
			if(!$mTbl->hasAnyWhere("id = $mIdParent"))
			{
				$mMsg = "Invalid parent ID: $mIdParent";
				return false;
			}

			return $mIdParent;
		}

		return 'null';
	}
}

?>