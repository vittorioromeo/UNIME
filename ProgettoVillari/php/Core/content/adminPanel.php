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
					<?php
						Gen::LinkBtn('btnDbEnable', 'glyphicon-ok', 'Enable/clear');
						Gen::LinkBtn('btnDbDisable', 'glyphicon-remove', 'Disable');
						Gen::LinkBtn('btnDbRefresh', 'glyphicon-refresh', 'Refresh page');
					?>
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
						<?php Gen::LinkBtn('btnScAdd', 'glyphicon-plus'); ?>
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
						<?php Gen::LinkBtn('btnScDel', 'glyphicon-minus'); ?>
						<?php Gen::LinkBtn('btnScDelRecursive', '', 'Recursive'); ?>
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
						<?php Gen::LinkBtn('btnGrAdd', 'glyphicon-plus'); ?>						
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
						<?php Gen::LinkBtn('btnGrDel', 'glyphicon-minus'); ?>
						<?php Gen::LinkBtn('btnGrDelRecursive', '', 'Recursive'); ?>
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
				<h4 class="modal-title">Add/edit user</h4>
				<div class="usIdDiv"></div>
			</div>
			<div class="modal-body">

				<?php
					Gen::Textbox("tbUsAddUsername", "Username");
					Gen::Textbox("tbUsAddEmail", "Email");
					Gen::Textbox("tbUsAddFirstname", "First name");
					Gen::Textbox("tbUsAddLastname", "Last name");
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
					<?php Gen::LinkBtn('btnUsAddOk', 'glyphicon-plus'); ?>					
					<?php Gen::BtnCloseModal(); ?>
				</div>
			</div>
		</div>
	</div>
</div>

<div class="modal fade" id="modalUsActions">
	<div class="modal-dialog">
		<div class="modal-content">
			<div class="modal-header">
				<button type="button" class="close" data-dismiss="modal" aria-label="Close">
					<span aria-hidden="true"></span>
				</button>
				<h4 class="modal-title">User actions</h4>
				<div class="usIdDiv"></div>
			</div>
			<div class="modal-body">

				<div class="btn-group">
					<?php Gen::LinkBtn('btnUsActionsUsDel', 'glyphicon-remove', 'Delete user'); ?>
					<?php Gen::LinkBtn('btnUsActionsUsResetPwd', 'glyphicon-envelope', 'Reset password'); ?>					
				</div>
			</div>
			<div class="modal-footer">
				<div class="btn-group pull-right">
					<?php Gen::LinkBtn('btnUsActionsOk', 'glyphicon-ok'); ?>					
					<?php Gen::BtnCloseModal(); ?>
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
		'showAPModal("Delete recursive", mOut);',
		'showAPModal("Delete recursive - error", mErr);');




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
		'showAPModal("Delete recursive", mOut);',
		'showAPModal("Delete recursive - error", mErr);');



	Gen::JS_PostAction('refreshUsers()', 'getTblUsers',
		array(),
		'$("#tblUsManage").html(mOut);',
		'showAPModal("Refresh users - error", mErr);');


	Gen::JS_PostAction('usAdd()', 'usAdd',
			array
			( 
				'id' => 'usEditModalId',
				'username' => '$("#tbUsAddUsername").val()',
				'email' => '$("#tbUsAddEmail").val()',
				'firstname' => '$("#tbUsAddFirstname").val()',
				'lastname' => '$("#tbUsAddLastname").val()',
				'birthdate' => '$("#dateUsAddBirth").val()',
				'groupId' => '$("#slUsAddGroup").val()'
			),
			'showAPModal("Add/edit", mOut);',
			'showAPModal("Add/edit - error", mErr);');

	Gen::JS_PostAction('usDel(mId)', 'usDel',
			array( 'id' => 'mId' ),
			'showAPModal("Delete", mOut);',
			'showAPModal("Delete - error", mErr);');

	Gen::JS_PostAction('usGetData(mId)', 'usGetData',
			array( 'id' => 'mId' ),
			'fillUsEditModal(mId, mOut);',
			'showAPModal("Get data - error", mErr);');


	Gen::JS_OnBtnClick('btnDbEnable', 			'setDebugMode(true);');
	Gen::JS_OnBtnClick('btnDbDisable', 			'setDebugMode(false);');
	Gen::JS_OnBtnClick('btnDbRefresh', 			'refreshAll();');

	Gen::JS_OnBtnClick('btnScAdd', 				'scAdd(); refreshAll();');
	Gen::JS_OnBtnClick('btnScDel', 				'scDel(); refreshAll();');
	Gen::JS_OnBtnClick('btnScDelRecursive', 	'scDelRecursive(); refreshAll();');

	Gen::JS_OnBtnClick('btnGrAdd', 				'grAdd(); refreshAll();');
	Gen::JS_OnBtnClick('btnGrDel', 				'grDel(); refreshAll();');
	Gen::JS_OnBtnClick('btnGrDelRecursive', 	'grDelRecursive(); refreshAll();');

	Gen::JS_OnBtnClickDynamic('btnUsAdd',		'showUsEditModal(-1);');

	Gen::JS_OnBtnClick('btnUsAddOk', 			'usAdd(); refreshAll();');
	Gen::JS_OnBtnClick('btnUsActionsUsDel', 	'usDel(usEditModalId); refreshAll();');
?>

<script>
	var usEditModalId = -1;

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

	function setUsEditId(mId)
	{
		usEditModalId = mId;

		if(mId != -1)
		{
			$(".usIdDiv").text("ID: " + usEditModalId);
		}
		else
		{
			$(".usIdDiv").text("New user");
		}
	}

	function fillUsEditModal(mId, mOut)
	{	
		var x = JSON.parse(mOut);

		$("#tbUsAddUsername").val(x["username"]);
		$("#tbUsAddEmail").val(x["email"]);
		$("#tbUsAddFirstname").val(x["firstname"]);
		$("#tbUsAddLastname").val(x["lastname"]);
		$("#dateUsAddBirth").val(x["birthdate"]);
		$("#slUsAddGroup").val(x["groupid"]);
	}

	function showUsEditModal(mId)
	{
		setUsEditId(mId);

		if(mId == -1)
		{
			$("#tbUsAddUsername").val("");
			$("#tbUsAddEmail").val("");
			$("#tbUsAddFirstname").val("");
			$("#tbUsAddLastname").val("");
			$("#dateUsAddBirth").val("")
		}
		else
		{
			usGetData(mId);
		}

		$("#modalUsAdd").modal("show");
	}

	function showUsActionsModal(mId)
	{
		setUsEditId(mId);
		$("#modalUsActions").modal("show");
	}

	$(document).ready(function(){ refreshAll(); });
</script>
