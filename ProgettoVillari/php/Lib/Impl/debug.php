<?php

class Debug
{
	public static function enable()
	{			
		Session::set(SKeys::$debugEnabled, true);
		Session::set(SKeys::$debugLog, "");
	}

	public static function disable()
	{
		Session::set(SKeys::$debugEnabled, false);
	}

	public static function isEnabled()
	{
		return Session::get(SKeys::$debugEnabled);
	}

	public static function lo($mX)
	{
		if(!Debug::isEnabled()) return;

		$value = Session::get(SKeys::$debugLog);
		$value .= $mX . '<br/>';
		Session::set(SKeys::$debugLog, $value);
	}

	public static function loLn()
	{
		if(!Debug::isEnabled()) return;

		$value = Session::get(SKeys::$debugLog);
		$value .= '<br/>-------------------<br/>';
		Session::set(SKeys::$debugLog, $value);		
	}

	public static function echoLo()
	{
		if(!Debug::isEnabled()) return;

		print('<br/><hr><br/>');
		print(Session::get(SKeys::$debugLog));
	}
}

?>