<?php

class Privs
{
	public static $count = 1;

	public static $superAdmin = 0;
}

class PrivSet
{
	private $bits = "";

	public function __construct()
	{
		$this->bits = str_repeat('F', Privs::$count);
	}

	public static function fromStr($mX)
	{
		$res = new PrivSet();
		$res->bits = $mX;
		return $res;
	}

	public function toStr()
	{
		return $this->bits;
	}

	public function add($mX)
	{
		$this->bits[$mX] = 'T';
	}

	public function del($mX)
	{
		$this->bits[$mX] = 'F';
	}

	public function has($mX)
	{
		return $this->bits[$mX] == 'T';
	}

	public function getOrWith($mX)
	{
		$res = new PrivSet();
		
		for($i = 0; $i < Privs::$count; $i++)
		{
			if($this->has($i) || $mX->has($i))
			{
				$res->add($i);
			}
		}

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

	public static function getPwdHash($mX)
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