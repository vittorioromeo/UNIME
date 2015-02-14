<?php

class Tbl
{
	protected $tblName;
	private $insertFields;

	public function __construct($mTblName) 
	{
		$this->tblName = $mTblName;
	}

	public function setInsertFields(...$mFields)
	{
		$this->insertFields = array();

		foreach($mFields as $x) 
		{
        	array_push($this->insertFields, $x);
    	}
	}

	public function insert(...$mValues)
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

	public function deleteWhere($mX)
	{
		return DB::query("DELETE FROM $this->tblName WHERE $mX");
	}

	public function getAllRows()
	{
		return DB::query("SELECT * FROM $this->tblName");
	}

	public function getAllRowsWhere($mX)
	{
		return DB::query("SELECT * FROM $this->tblName WHERE $mX");
	}

	public function hasAnyWhere($mX)
	{
		return count($this->getAllRowsWhere($mX)) > 0;
	}

	public function deleteWithChildren($mId, &$mMsg)
	{
		$qres = DB::query("SELECT * FROM $this->tblName WHERE id_parent = $mId");

		if($qres == null)
		{
			$mMsg = "Error! " . DB::$lastError;
			return false;
		}

		while($row = $qres->fetch_assoc()) 
		{
			$this->deleteWithChildren($row["id"], $mMsg);
		}

		$this->deleteWhere("id = $mId");
		$mMsg = "Success.";
		return true;
	}	

	public function forHierarchy($mFn, &$mMsg, $mParent = 'null', $mDepth = 0)
	{
		$res = "";
		$where = "";
		if($mParent == 'null') 
		{
			$where = "id_parent IS NULL";
		}
		else
		{
			$where = "id_parent = $mParent";
		}

		$qres = DB::query("SELECT * FROM $this->tblName WHERE $where");

		if($qres == null)
		{
			$mMsg = "Error getting $this->tblName hierarchy. " . DB::$lastError;
			return false;
		}

		while($row = $qres->fetch_assoc()) 
		{
			$nextId = $mFn($row, $mDepth);
			$this->forHierarchy($mFn, $mMsg, $nextId, $mDepth + 1);
		}

		$mMsg = "Success getting $this->tblName hierarchy.";
		return true;
	}
}

?>