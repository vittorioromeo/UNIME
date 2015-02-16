<?php

class Debug
{
	public static function enable()
	{			
		Session::set(SK::$debugEnabled, true);
		Session::set(SK::$debugLog, "");
	}

	public static function disable()
	{
		Session::set(SK::$debugEnabled, false);
	}

	public static function isEnabled()
	{
		return Session::get(SK::$debugEnabled);
	}

	public static function lo($mX)
	{
		if(!Debug::isEnabled()) return;

		$value = Session::get(SK::$debugLog);
		$value .= $mX . '<br/>';
		Session::set(SK::$debugLog, $value);
	}

	public static function loLn()
	{
		if(!Debug::isEnabled()) return;

		$value = Session::get(SK::$debugLog);
		$value .= '<br/>-------------------<br/>';
		Session::set(SK::$debugLog, $value);		
	}

	public static function echoLo()
	{
		if(!Debug::isEnabled()) return;

		print('<br/><hr><br/>');
		print(Session::get(SK::$debugLog));
	}
}

?>