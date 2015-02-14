<h1>Administration</h1>

<div class="row">
	<div class="col-md-2">
		<h2>Debug</h2>
		<div class="panel panel-default">
			<div class="panel-heading"><h4 class="panel-title">Actions</h4></div>
			<div class="panel-body">
				<div class="btn-group-vertical pull-right">		
					<a class="btn btn-default" href="#" role="button" id="btnDebugEnable">
						<span class="glyphicon glyphicon-ok" aria-hidden="true"></span>
						Enable/clear
					</a>
					<a class="btn btn-default" href="#" role="button" id="btnDebugDisable">
						<span class="glyphicon glyphicon-remove" aria-hidden="true"></span>
						Disable
					</a>
					<a class="btn btn-default" href="#" role="button" id="btnRefresh">
						<span class="glyphicon glyphicon-refresh" aria-hidden="true"></span>
						Refresh page
					</a>
				</div>
			</div>
		</div>		
	</div>
	<div class="col-md-8">
		<h2>Sections</h2>
		<div class="col-md-4">
			<div class="panel panel-default">
				<div class="panel-heading"><h4 class="panel-title">Create section</h4></div>
				<div class="panel-body">
					<form id="formAddSection" action="action_createSection.php">
					
						<?php Gen::Textbox("tbName", "Section name"); ?>

						<div class="form-group">
							<label for="slParent">Parent section</label>
							<select class="form-control" name="slParent" id="slParent">
								
							</select>
						</div>

						<div class="btn-group pull-right">
							<button type="button" id="btnCreate" class="btn btn-default">
								<span class="glyphicon glyphicon-plus" aria-hidden="true"></span>
								Create
							</button> 
						</div>
					</form>
				</div>
			</div>
		</div>
		<div class="col-md-4">
			<div class="panel panel-default">
				<div class="panel-heading"><h4 class="panel-title">Delete section</h4></div>
				<div class="panel-body">
					<form id="formDelSection" action="action_delSection.php">									
						<div class="form-group">
							<label for="slParent">Select section</label>
							<select class="form-control" name="slToDel" id="slToDel">
								
							</select>
						</div>

						<div class="btn-group-vertical pull-right">							
							<a role="button" id="btnDel" class="btn btn-default">
								<span class="glyphicon glyphicon-minus" aria-hidden="true"></span>
								Delete
							</a>
							<a role="button" id="btnDelWithChildren" class="btn btn-default">
								<span class="glyphicon glyphicon-remove" aria-hidden="true"></span>
								Delete recursively
							</a>
						</div>
					</form>
				</div>
			</div>
		</div>
		<div class="col-md-4">
			<div id="sectionHierarchy"></div>
		</div>
	</div>
	
</div>

<hr>

<?php 
	Gen::PostJSAction('setDebugEnabled(mX)', 'setDebugEnabled', array( 'enabled' => 'mX' ));

	Gen::PostJSAction('refreshDebugLo()', 'refreshDebugLo', array(), '{ $("#debugLo").html(mOut); }');

	Gen::PostJSAction('refreshSectionHierarchy()', 'getSectionHierarchyStr', array(), '{ $("#sectionHierarchy").html(mOut); }');

	Gen::PostJSAction('refreshSections(mTarget, mNullRow)', 'getSectionOptions',
		array( 'nullRow' => 'mNullRow' ),
		'{ $(mTarget).html(mOut); }');

	Gen::PostJSAction('createSection()', 'createSection',
		array( 'idParent' => '$("#slParent").val()', 'name' => '$("#tbName").val()' ),
		'{ showModalInfo("Create", mOut); }',
		'{ showModalInfo("Create - error", mErr); }');

	Gen::PostJSAction('deleteSection()', 'deleteSection',
		array( 'id' => '$("#slToDel").val()' ),
		'{ showModalInfo("Delete", mOut); }',
		'{ showModalInfo("Delete - error", mErr); }');

	Gen::PostJSAction('deleteSectionWithChildren()', 'deleteSectionWithChildren',
		array( 'id' => '$("#slToDel").val()' ),
		'{ showModalInfo("Delete with children", mOut); }',
		'{ showModalInfo("Delete with children - error", mErr); }');
?>

<script>
	function refreshAll()
	{
		refreshSections("#slParent", true); 
		refreshSections("#slToDel", false); 
		refreshSectionHierarchy();
		refreshDebugLo(); 
	}

	function setDebugMode(mX)
	{
		setDebugEnabled(mX);
		refreshDebugLo();
		showModalInfo("Info", "Debug mode " + (mX ? "enabled." : "disabled."));
	}

	$(document).ready(function()
	{ 
		refreshAll();
	});

	$("#btnRefresh").click(function()
	{ 
		refreshAll();
		showModalInfo("Info", "Refresh successful.");
	});

	$("#btnCreate").click(function()
	{
		createSection();
		refreshAll();
		//showModalInfo("Info", "Section created successful.");
	});

	$("#btnDel").click(function()
	{
		deleteSection();
		refreshAll();
		//showModalInfo("Info", "Section deletion successful.");
	});

	$("#btnDelWithChildren").click(function()
	{
		deleteSectionWithChildren();
		refreshAll();
		//showModalInfo("Info", "Section deletion successful.");
	});

	$("#btnDebugEnable").click(function(){ setDebugMode(true); });
	$("#btnDebugDisable").click(function(){ setDebugMode(false); });

</script>