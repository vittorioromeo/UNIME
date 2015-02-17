<?php

class Credentials
{
	public static function isLoggedIn()
	{
		return Session::get(SK::$userID) != null;
	}

	public static function tryLogin($mUser, $mPass)
	{
		$row = TBS::$user->findByCredentials($mUser, $mPass);

		if(!$row)
		{
			Debug::lo("Invalid credentials: $mUser, $mPass");
			Session::set(SK::$userID, null);
			return false;
		}
		
		Debug::lo("Login successful: $mUser, $mPass");			
		Session::set(SK::$userID, $row['id']);		
		Pages::setCurrent(PK::$sections);
		return true;		
	}

	public static function tryLogout()
	{		
		Session::set(SK::$userID, null);
		return true;
	}

	public static function getCUID()
	{
		return Session::get(SK::$userID);
	}

	public static function getCURow()
	{
		return TBS::$user->findByID(Credentials::getCUID());
	}

	public static function getCalcPSet()
	{
		$groupID = Credentials::getCURow()['id_group'];
		$group = TBS::$group->findByID($groupID);		

		$calcPset = PrivSet::fromStr($group['privileges']);

		TBS::$group->forParent(function(&$mRow, $mDepth) use (&$calcPset)
		{
			$calcPset = $calcPset->getOrWith(PrivSet::fromStr($mRow['privileges']));
		}, $groupID);

		return $calcPset;
	}

	public static function hasCUPrivilege($mX)
	{
		return Credentials::getCalcPSet()->has($mX);
	}

	public static function canCUViewCurrentPage()
	{
		return Pages::getCurrent()->canViewWithPrivs(Credentials::getCalcPSet());
	}
}

?>