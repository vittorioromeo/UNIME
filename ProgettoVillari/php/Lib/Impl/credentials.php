<?php

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
			return false;
		}
		else
		{
			Debug::lo("Login successful: $mUser, $mPass");
			Session::set(SKeys::$loggedIn, true);
			Session::set(SKeys::$userID, $userID);
			return true;
		}
	}

	public static function tryLogout()
	{
		Session::set(SKeys::$loggedIn, false);
		Session::set(SKeys::$userID, null);
		return true;
	}

	public static function getCURow()
	{
		return Tables::$user->findByID(Session::get(SKeys::$userID));
	}

	public static function getCalcPSet()
	{
		$groupID = Credentials::getCURow()['id_group'];
		$group = Tables::$group->findByID($groupID);
		$msg = "";

		$calcPset = PrivSet::fromStr($group['privileges']);

		Tables::$group->forChildren(function($mRow, $mDepth) use (&$calcPset)
		{
			$id = $mRow['id_parent'];	
			$privileges = $mRow['privileges'];

			$calcPset = $calcPset->getOrWith(PrivSet::fromStr($privileges));

			return $id;
		}, $msg, $groupID);

		return $calcPset;
	}

	public static function hasCUPrivilege($mX)
	{
		return Credentials::getCalcPSet()->has($mX);
	}
}

?>