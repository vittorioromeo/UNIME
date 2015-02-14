<?php
	$root = realpath($_SERVER["DOCUMENT_ROOT"]) . "/PV";
	require_once("$root/php/Lib/lib.php");
?>

<h1>Administration</h1>

<div class="row">
	<div class="col-md-4">
		<h2>Debug</h2>
		<div class="panel panel-default">
			<div class="panel-heading"><h4 class="panel-title">Actions</h4></div>
			<div class="panel-body">
				<div class="btn-group-vertical pull-right">		
					<a class="btn btn-default" href="#" role="button" id="btnDbEnable">
						<span class="glyphicon glyphicon-ok" aria-hidden="true"></span>
						Enable/clear
					</a>
					<a class="btn btn-default" href="#" role="button" id="btnDbDisable">
						<span class="glyphicon glyphicon-remove" aria-hidden="true"></span>
						Disable
					</a>
					<a class="btn btn-default" href="#" role="button" id="btnDbRefresh">
						<span class="glyphicon glyphicon-refresh" aria-hidden="true"></span>
						Refresh page
					</a>
				</div>
				<div class="checkbox">
					<label>
						<input id="cbDbModals" type="checkbox" checked="true"> 
						Show modals in this page
					</label>
				</div>
			</div>
		</div>		
	</div>
	<div class="col-md-8">
		<h2>Sections</h2>
		<div class="col-md-4">
			<div class="panel panel-default">
				<div class="panel-heading"><h4 class="panel-title">Add</h4></div>
				<div class="panel-body">
					<?php Gen::Textbox("tbScName", "Name"); ?>

					<div class="form-group">
						<label for="slScParent">Parent</label>
						<select class="form-control" id="slScParent">
							
						</select>
					</div>

					<div class="btn-group pull-right">
						<button type="button" id="btnScAdd" class="btn btn-default">
							<span class="glyphicon glyphicon-plus" aria-hidden="true"></span>
						</button> 
					</div>				
				</div>
			</div>
		</div>
		<div class="col-md-4">
			<div class="panel panel-default">
				<div class="panel-heading"><h4 class="panel-title">Delete</h4></div>
				<div class="panel-body">
					<div class="form-group">
						<label for="scScToDel">Select</label>
						<select class="form-control" id="scScToDel">
							
						</select>
					</div>

					<div class="btn-group pull-right">							
						<a role="button" id="btnScDel" class="btn btn-default">
							<span class="glyphicon glyphicon-minus" aria-hidden="true"></span>							
						</a>
						<a role="button" id="btnScDelRecursive" class="btn btn-default">								
							Recursive
						</a>
					</div>
				</div>
			</div>
		</div>
		<div class="col-md-4">
			<div class="panel panel-default">
				<div class="panel-heading"><h4 class="panel-title">Hierarchy</h4></div>
				<div class="panel-body">
					<div id="divScHierarchy"></div>
				</div>
			</div>
		</div>
	</div>
</div>

<hr>

<div class="row">
	<div class="col-md-4">
		<h2>Tags</h2>
		<div class="panel panel-default">
			<div class="panel-heading"><h4 class="panel-title">TODO</h4></div>
			<div class="panel-body">	
				TODO
			</div>
		</div>
	</div>

	<div class="col-md-8">
		<h2>Groups</h2>
		<div class="col-md-4">
			<div class="panel panel-default">
				<div class="panel-heading"><h4 class="panel-title">Add</h4></div>
				<div class="panel-body">					
					<?php Gen::Textbox("tbGrName", "Name"); ?>

					<div class="form-group">
						<label for="slGrPrivileges">Privileges</label>
						<select class="form-control" multiple="true" id="slGrPrivileges">
							<option value="0">SuperAdmin</option>							
						</select>
					</div>

					<div class="form-group">
						<label for="slGrParent">Parent</label>
						<select class="form-control" id="slGrParent">
							
						</select>
					</div>

					<div class="btn-group pull-right">
						<button type="button" id="btnGrAdd" class="btn btn-default">
							<span class="glyphicon glyphicon-plus" aria-hidden="true"></span>
						</button> 
					</div>
				</div>
			</div>
		</div>
		<div class="col-md-4">
			<div class="panel panel-default">
				<div class="panel-heading"><h4 class="panel-title">Delete</h4></div>
				<div class="panel-body">					
					<div class="form-group">
						<label for="slGrToDel">Select</label>
						<select class="form-control" name="slGrToDel" id="slGrToDel">
							
						</select>
					</div>

					<div class="btn-group pull-right">							
						<a role="button" id="btnGrDel" class="btn btn-default">
							<span class="glyphicon glyphicon-minus" aria-hidden="true"></span>								
						</a>
						<a role="button" id="btnGrDelRecursive" class="btn btn-default">								
							Recursive
						</a>
					</div>
				</div>
			</div>
		</div>
		<div class="col-md-4">
			<div class="panel panel-default">
				<div class="panel-heading"><h4 class="panel-title">Hierarchy</h4></div>
				<div class="panel-body">
					<div id="divGrHierarchy"></div>
				</div>
			</div>
		</div>
	</div>
</div>

<hr>

<div class="row">
	<div class="col-md-12">
		<h2>Users</h2>
		<div class="panel panel-default">
			<div class="panel-heading"><h4 class="panel-title">Manage</h4></div>
			<div class="panel-body">
				<table id="tblUsManage" class="table table-bordered table-striped">
					
				</table>
			</div>
		</div>		
	</div>
	
</div>

<div class="modal fade" id="modalUsAdd">
	<div class="modal-dialog">
		<div class="modal-content">
			<div class="modal-header">
				<button type="button" class="close" data-dismiss="modal" aria-label="Close">
					<span aria-hidden="true"></span>
				</button>
				<h4 class="modal-title">Add user</h4>
			</div>	
			<div class="modal-body">
				<?php 
					Gen::Textbox("tbUsAddUsername", "Username"); 
					Gen::Textbox("tbUsAddEmail", "Email"); 
					Gen::Textbox("tbUsAddFirstname", "First name"); 
					Gen::Textbox("tbUsAddLastname", "Last name"); 
					//Gen::Textbox("tbUsAddGroup", "Group ID"); 
					//Gen::Textbox("tbUsAddRegistrationDate", "Registration date"); 
					//Gen::Textbox("tbUsAddBirthDate", "Birth date"); 
				?>

				<div class="form-group">
					<label for="dateUsAddBirth">Birth date</label>
					<input class="form-control" type="date" id="dateUsAddBirth">
				</div>

				<div class="form-group">
					<label for="slUsAddGroup">Group</label>
					<select class="form-control" name="slUsAddGroup" id="slUsAddGroup">
							
					</select>
				</div>
			</div>
			<div class="modal-footer">
				<div class="btn-group pull-right">
					<button type="button" class="btn btn-default" id="btnUsAddOk">
						<span class="glyphicon glyphicon-plus" aria-hidden="true"></span>	
					</button>  
					<button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
				</div>   
			</div>
		</div>
	</div>
</div>

<hr>

<?php 
	Gen::JS_PostAction('setDebugEnabled(mX)', 'setDebugEnabled', array( 'enabled' => 'mX' ));

	Gen::JS_PostAction('refreshDebugLo()', 'refreshDebugLo', array(), '$("#debugLo").html(mOut);');



	Gen::JS_PostAction('refreshSectionHierarchy()', 'getSectionHierarchyStr', array(), '$("#divScHierarchy").html(mOut);');

	Gen::JS_PostAction('refreshSections(mTarget, mNullRow)', 'getSectionOptions',
		array( 'nullRow' => 'mNullRow' ),
		'$(mTarget).html(mOut);');

	Gen::JS_PostAction('scAdd()', 'scAdd',
		array( 'idParent' => '$("#slScParent").val()', 'name' => '$("#tbScName").val()' ),
		'showAPModal("Create", mOut);',
		'showAPModal("Create - error", mErr);');

	Gen::JS_PostAction('scDel()', 'scDel',
		array( 'id' => '$("#scScToDel").val()' ),
		'showAPModal("Delete", mOut);',
		'showAPModal("Delete - error", mErr);');

	Gen::JS_PostAction('scDelRecursive()', 'scDelRecursive',
		array( 'id' => '$("#scScToDel").val()' ),
		'showAPModal("Delete with children", mOut);',
		'showAPModal("Delete with children - error", mErr);');




	Gen::JS_PostAction('refreshGroupHierarchy()', 'getGroupHierarchyStr', array(), '$("#divGrHierarchy").html(mOut);');

	Gen::JS_PostAction('refreshGroups(mTarget, mNullRow)', 'getGroupOptions',
		array( 'nullRow' => 'mNullRow' ),
		'$(mTarget).html(mOut);');

	Gen::JS_PostAction('grAdd()', 'grAdd',
		array
		( 
			'idParent' => '$("#slGrParent").val()', 
			'name' => '$("#tbGrName").val()',
			'privileges' => '$("#slGrPrivileges").val()'
		),
		'showAPModal("Create", mOut);',
		'showAPModal("Create - error", mErr);');

	Gen::JS_PostAction('grDel()', 'grDel',
		array( 'id' => '$("#slGrToDel").val()' ),
		'showAPModal("Delete", mOut);',
		'showAPModal("Delete - error", mErr);');

	Gen::JS_PostAction('grDelRecursive()', 'grDelRecursive',
		array( 'id' => '$("#slGrToDel").val()' ),
		'showAPModal("Delete with children", mOut);',
		'showAPModal("Delete with children - error", mErr);');



	Gen::JS_PostAction('refreshUsers()', 'getTblUsers',
		array(),
		'$("#tblUsManage").html(mOut);',
		'showAPModal("Refresh users - error", mErr);');

	Gen::JS_OnBtnClick('btnDbEnable', 			'setDebugMode(true);');
	Gen::JS_OnBtnClick('btnDbDisable', 			'setDebugMode(false);');
	Gen::JS_OnBtnClick('btnDbRefresh', 			'refreshAll();');

	Gen::JS_OnBtnClick('btnScAdd', 				'scAdd(); refreshAll();');
	Gen::JS_OnBtnClick('btnScDel', 				'scDel(); refreshAll();');
	Gen::JS_OnBtnClick('btnScDelRecursive', 	'scDelRecursive(); refreshAll();');

	Gen::JS_OnBtnClick('btnGrAdd', 				'grAdd(); refreshAll();');
	Gen::JS_OnBtnClick('btnGrDel', 				'grDel(); refreshAll();');
	Gen::JS_OnBtnClick('btnGrDelRecursive', 	'grDelRecursive(); refreshAll();');

	Gen::JS_OnBtnClickDynamic('btnUsAdd',		'$("#modalUsAdd").modal("show");');

	Gen::JS_OnBtnClick('btnUsAddOk', 			' /*todo*/ ');
?>

<script>
	function refreshAll()
	{
		refreshSections("#slScParent", true); 
		refreshSections("#slScToDel", false); 
		refreshSectionHierarchy();

		refreshGroups("#slGrParent", true); 
		refreshGroups("#slGrToDel", false); 
		refreshGroups("#slUsAddGroup", false);
		refreshGroupHierarchy();

		refreshUsers();
		
		refreshDebugLo(); 
	}

	function showAPModal(mHeading, mBody)
	{
		if($('#cbDbModals').prop('checked'))
			showModalInfo(mHeading, mBody);
	}

	function setDebugMode(mX)
	{
		setDebugEnabled(mX);
		refreshDebugLo();
		showAPModal("Info", "Debug mode " + (mX ? "enabled." : "disabled."));
	}

	$(document).ready(function(){ refreshAll(); });
</script>	