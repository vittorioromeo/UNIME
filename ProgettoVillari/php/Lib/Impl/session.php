<?php

class SKeys
{
	public static $loggedIn = "A0";
	public static $userID = "A1";
	public static $debugLog = "A2";
	public static $debugEnabled = "A3";
}

class Session
{
	public static function init()
	{
		session_start();
	}

	public static function get($mX)
	{
		if(!isset($_SESSION[$mX])) 
		{
			// Debug::lo("Getting session $mX (null)");
			return null;
		}
		
		$value = $_SESSION[$mX];

		// Debug::lo("Getting session $mX ($value)");
		return $value;
	}

	public static function isNull($mX)
	{
		if(!isset($_SESSION[$mX])) return true;
		return empty($_SESSION[$mX]) || $_SESSION[$mX] == null; 
	}

	public static function set($mX, $mVal)
	{
		// Debug::lo("Setting session $mX to $mVal");
		$_SESSION[$mX] = $mVal;
	}	
}

?>