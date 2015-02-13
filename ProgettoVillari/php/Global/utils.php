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

class Gen
{
	public static function BSPanelStart($mHeading)
	{
		print('
		<div class="panel panel-default">
		  	<div class="panel-heading">' . $mHeading . '</div>
		  	<div class="panel-body">
		');
	}

	public static function BSPanelEnd()
	{
		print('</div></div>');
	}

	public static function Textbox($mID, $mLabel)
	{
		print("
		<div class=\"form-group\">
			<label for=\"$mID\">$mLabel</label>
			<input type=\"text\" class=\"form-control\" id=\"$mID\" placeholder=\"$mLabel\">
		</div>
		");
	}
}

?>