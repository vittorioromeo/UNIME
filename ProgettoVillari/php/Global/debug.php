<?php

class Debug
{
	public static $str = "";

	public static function lo($mX)
	{
		Debug::$str .= $mX . "<br/>";
	}

	public static function loLn()
	{
		Debug::$str .= "<br/>-------------------<br/>";
	}

	public static function echoLo()
	{
		echo Debug::$str;
	}
}

?>