<?php

class Gen
{

	public static function Textbox($mID, $mLabel)
	{
		print("
		<div class=\"form-group\">
			<label for=\"$mID\">$mLabel</label>
			<input type=\"text\" class=\"form-control\" id=\"$mID\" placeholder=\"$mLabel\">
		</div>
		");
	}

	public static function LinkIcon($mID, $mIcon)
	{
		print('
		<a id="'.$mID.'">
			<span class="glyphicon '.$mIcon.'" aria-hidden="true"></span>
		</a>
		');
	}

	public static function PostJSAction($mFnName, $mAction, $mKVArray, $mFnOk = '{}', $mFnFail = '{}')
	{
		print('
		<script>
			function '.$mFnName.'
			{
				var url = "php/Core/content/actions.php";
				var sentData = 
				{ 
					action: '."\"$mAction\"");
		
		foreach($mKVArray as $k => $v)
		{
			print(', '.$k.': '.$v);			
		}
		
		print('
				};

				$.post(url, sentData, 
					function(mOut, mTS, mJQXHR)'.$mFnOk.')
				.fail(
					function(mJQXHR, mTS, mErr)'.$mFnFail.');
			}
		</script>
		');
	}
}

?>
