<?php

class Tbl
{
	private $tblName;
	private $insertFields;

	function __construct($mTblName) 
	{
		$this->tblName = $mTblName;
	}

	function setInsertFields(...$mFields)
	{
		$this->insertFields = array();

		foreach($mFields as $x) 
		{
        	array_push($this->insertFields, $x);
    	}
	}

	function insert(...$mValues)
	{
		$insertFieldsStr = Utils::GetCSL($this->insertFields);
		$insertValuesStr = Utils::GetCSL($mValues);
		
		Debug::loLn();
		Debug::lo("Inserting into table $this->tblName:");
		Debug::lo("FIELDS: $insertFieldsStr");
		Debug::lo("VALUES: $insertValuesStr");

		if(count($mValues) != count($this->insertFields))
		{
			Debug::lo("Error: count of values does not match count of fields.");
			return null;
		}

		$res = DB::query("INSERT INTO $this->tblName ($insertFieldsStr) VALUES ($insertValuesStr);");
		return $res;
	}

	function getAllRows()
	{
		return DB::query("SELECT * FROM $this->tblName");
	}

	function getAllRowsWhere($mX)
	{
		return DB::query("SELECT * FROM $this->tblName WHERE $mX");
	}

	function hasAnyWhere($mX)
	{
		return count($this->getAllRowsWhere($mX)) > 0;
	}
}

?>