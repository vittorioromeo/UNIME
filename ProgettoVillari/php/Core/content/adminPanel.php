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

<hr>

<div class="row">
	<div class="col-md-4">
		<h2>Group-section permissions</h2>
		<div class="panel panel-default">
			<div class="panel-heading"><h4 class="panel-title">Manage</h4></div>
			<div class="panel-body">
				<div class="form-group">
					<label for="slGSPGr">Group</label>
					<select class="form-control" id="slGSPGr">

					</select>
				</div>
				<div class="form-group">
					<label for="slGSPSc">Section</label>
					<select class="form-control" id="slGSPSc">

					</select>
				</div>		
				<div class="btn-group pull-right">
					<?php Gen::LinkBtn('btnGSPModal', 'glyphicon-arrow-right'); ?>				
				</div>		
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
				?>
				
				<span id="modalUsAddPwd">
					<?php Gen::Textbox("tbUsAddPassword", "Password"); ?>
				</span>

				<?php
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

<div class="modal fade" id="modalGSPerms">
	<div class="modal-dialog">
		<div class="modal-content">
			<div class="modal-header">
				<button type="button" class="close" data-dismiss="modal" aria-label="Close">
					<span aria-hidden="true"></span>
				</button>
				<h4 class="modal-title">Permissions</h4>
				<div class="gspNameDiv"></div>
			</div>
			<div class="modal-body">
				<div class="col-md-6">
					<?php 
						Gen::CheckBox("cbGSPCView", "Can view");
						Gen::CheckBox("cbGSPCPost", "Can post");
						Gen::CheckBox("cbGSPCreateThread", "Can create thread");
					?>
				</div>	
				<div class="col-md-6">
					<?php 
						Gen::CheckBox("cbGSPCDeletePost", "Can delete post");
						Gen::CheckBox("cbGSPCDeleteThread", "Can delete thread");
						Gen::CheckBox("cbGSPCDeleteSection", "Can section");
					?>
				</div>	
			</div>
			<div class="modal-footer">
				<div class="btn-group pull-right">
					<?php Gen::LinkBtn('btnGSPModalOK', 'glyphicon-ok'); ?>					
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
				'password' => '$("#tbUsAddPassword").val()',
				'email' => '$("#tbUsAddEmail").val()',
				'firstname' => '$("#tbUsAddFirstname").val()',
				'lastname' => '$("#tbUsAddLastname").val()',
				'birthdate' => '$("#dateUsAddBirth").val()',
				'groupId' => '$("#slUsAddGroup").val()'
			),
			'showAPModal("Add/edit", mOut);',
			'showAPModal("Add/edit - error", mErr);');

	Gen::JS_PostAction('usDel(mID)', 'usDel',
			array( 'id' => 'mID' ),
			'showAPModal("Delete", mOut);',
			'showAPModal("Delete - error", mErr);');

	Gen::JS_PostAction('usGetData(mID)', 'usGetData',
			array( 'id' => 'mID' ),
			'fillUsEditModal(mID, mOut);',
			'showAPModal("Get data - error", mErr);');


	Gen::JS_PostAction('startGSPEdit(mIDGroup, mIDSection)', 'getGSPData',
			array( 'idgroup' => 'mIDGroup', 'idsection' => 'mIDSection' ),
			'fillGSPModal(mOut); showGSPModal(mIDGroup, mIDSection);',
			'showAPModal("Get data - error", mErr);');

	Gen::JS_PostAction('endGSPEdit(mIDGroup, mIDSection, mA0, mA1, mA2, mA3, mA4, mA5)', 'setGSPData',
			array
			( 
				'idgroup' => 'mIDGroup', 
				'idsection' => 'mIDSection',

				'cpost' => 'mA0',
				'cview' => 'mA1',
				'ccreatethread' => 'mA2',
				'cdeletepost' => 'mA3',
				'cdeletethread' => 'mA4',
				'cdeletesection' => 'mA5'
			),
			'',
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

	Gen::JS_OnBtnClick('btnGSPModal',			'startGSPEdit($("#slGSPGr").val(), $("#slGSPSc").val());');
	Gen::JS_OnBtnClick('btnGSPModalOK', 		
		'endGSPEdit
		(
			$("#slGSPGr").val(),
			$("#slGSPSc").val(),
			$("#cbGSPCPost").prop("checked"),
			$("#cbGSPCView").prop("checked"),
			$("#cbGSPCreateThread").prop("checked"),
			$("#cbGSPCDeletePost").prop("checked"),
			$("#cbGSPCDeleteThread").prop("checked"),
			$("#cbGSPCDeleteSection").prop("checked")
		);

		$("#modalGSPerms").modal("hide");');
?>

<script>
	var usEditModalId = -1;

	function refreshAll()
	{
		refreshSections("#slScParent", true);
		refreshSections("#slScToDel", false);
		refreshSections("#slGSPSc", false);
		refreshSectionHierarchy();

		refreshGroups("#slGrParent", true);
		refreshGroups("#slGrToDel", false);
		refreshGroups("#slUsAddGroup", false);
		refreshGroups("#slGSPGr", false);
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

	function setUsEditId(mID)
	{
		usEditModalId = mID;

		if(mID != -1)
		{
			$(".usIdDiv").text("ID: " + usEditModalId);
		}
		else
		{
			$(".usIdDiv").text("New user");
		}
	}

	function fillUsEditModal(mID, mOut)
	{	
		var x = JSON.parse(mOut);

		$("#tbUsAddUsername").val(x["username"]);
		$("#tbUsAddEmail").val(x["email"]);
		$("#tbUsAddFirstname").val(x["firstname"]);
		$("#tbUsAddLastname").val(x["lastname"]);
		$("#dateUsAddBirth").val(x["birthdate"]);
		$("#slUsAddGroup").val(x["groupid"]);
	}


	function fillGSPModal(mOut)
	{	
		var x = JSON.parse(mOut);

		$("#cbGSPCPost").prop("checked", x["cpost"] == "1");
		$("#cbGSPCView").prop("checked", x["cview"] == "1");
		$("#cbGSPCreateThread").prop("checked", x["ccreatethread"] == "1");
		$("#cbGSPCDeletePost").prop("checked", x["cdeletepost"] == "1");
		$("#cbGSPCDeleteThread").prop("checked", x["cdeletethread"] == "1");
		$("#cbGSPCDeleteSection").prop("checked", x["cdeletesection"] == "1");
	}

	function showUsEditModal(mID)
	{
		setUsEditId(mID);

		if(mID == -1)
		{
			$("#modalUsAddPwd").show();

			$("#tbUsAddUsername").val("");
			$("#tbUsAddPassword").val("");
			$("#tbUsAddEmail").val("");
			$("#tbUsAddFirstname").val("");
			$("#tbUsAddLastname").val("");
			$("#dateUsAddBirth").val("")
		}
		else
		{
			$("#modalUsAddPwd").hide();
			usGetData(mID);
		}

		$("#modalUsAdd").modal("show");
	}

	function showGSPModal(mIDGroup, mIDSection)
	{
		$(".gspNameDiv").text("ID Group: " + mIDGroup + " || ID Section: " + mIDSection);
		$("#modalGSPerms").modal("show");
	}

	function showUsActionsModal(mID)
	{
		setUsEditId(mID);
		$("#modalUsActions").modal("show");
	}

	$(document).ready(function(){ refreshAll(); });
</script>
