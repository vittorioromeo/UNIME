<?php

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
}

?>