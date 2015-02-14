<?php

class DB
{
	public static $conn;
	public static $lastError;

	public static function connect()
	{
		DB::$conn = new mysqli("localhost", "root", "root", "db_forum");

		if(DB::$conn->connect_errno) 
		{
		    Debug::lo("Failed to connect to MySQL: (" . DB::$conn->connect_errno . ") " . DB::$conn->connect_error);
		}
	}

	public static function query($mX)
	{
		Debug::loLn();
		Debug::lo("Executing query: " . $mX);

		$res = DB::$conn->query($mX);

		if(!$res)
		{
			DB::$lastError = "Query failed: (" . DB::$conn->errno . ") " . DB::$conn->error;
			Debug::lo(DB::$lastError);			
			return null;
		}

		return $res;
	}

	public static function esc($mX)
	{
		return DB::$conn->real_escape_string($mX);
	}

	public static function v($mX)
	{
		$res = DB::esc($mX);
		return "'$res'";
	}
}

DB::connect();
Debug::lo(DB::$conn->host_info . "\n");

?>