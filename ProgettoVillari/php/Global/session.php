<?php

class SKeys
{
	public static $loggedIn = "A0";
	public static $userID = "A1";
}

class Session
{
	public static function init()
	{
		session_start();
	}

	public static function get($mX)
	{
		if(isset($_SESSION[$mX])) return null;
		$value = $_SESSION[$mX];

		Debug::lo("Getting session $mX ($value)");
		return $value;
	}

	public static function isNull($mX)
	{
		if(!isset($_SESSION[$mX])) return true;
		return empty($_SESSION[$mX]) || $_SESSION[$mX] == null; 
	}

	public static function set($mX, $mVal)
	{
		Debug::lo("Setting session $mX to $mVal");
		$_SESSION[$mX] = $mVal;
	}
}

class Credentials
{
	public static function isLoggedIn()
	{
		if(Session::isNull(SKeys::$loggedIn)) 
		{
			Debug::lo("Not logged in");
			return false;
		}


		$value = Session::get(SKeys::$loggedIn);

		return $value == true;

	}
	public static function tryLogin($mUser, $mPass)
	{
		$userID = Tables::$user->findIDByCredentials($mUser, $mPass);

		if($userID == -1)
		{
			Debug::lo("Invalid credentials: $mUser, $mPass");
		}
		else
		{
			Debug::lo("Login successful: $mUser, $mPass");
			Session::set(SKeys::$loggedIn, true);
			Session::set(SKeys::$userID, $userID);
		}
	}
}

?>