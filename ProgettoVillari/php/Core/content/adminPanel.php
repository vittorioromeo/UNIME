<h1>Administration</h1>
<?php Gen::LinkIcon("btnRefresh", "glyphicon-refresh"); ?>

<div class="row">
	<div class="col-md-2">
		<h2>Debugging</h2>
		<div class="panel panel-default">
			<div class="panel-heading"><h4 class="panel-title">Actions</h4></div>
			<div class="panel-body">
				<a class="btn btn-default" href="#" role="button" id="btnDebugEnable">Enable/clear</a>
				<a class="btn btn-default" href="#" role="button" id="btnDebugDisable">Disable</a>
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

						<div class="btn-group pull-right">
							<button type="button" id="btnDelete" class="btn btn-default">
								<span class="glyphicon glyphicon-minus" aria-hidden="true"></span>
								Delete
							</button> 
						</div>
					</form>
				</div>
			</div>
		</div>
	</div>
	
</div>

<div class="modal fade" id="modalInfo">
	<div class="modal-dialog">
		<div class="modal-content">
			<div class="modal-header">
				<button type="button" class="close" data-dismiss="modal" aria-label="Close">
					<span aria-hidden="true"></span>
				</button>
				<h4 class="modal-title" id="modalInfoHeader"></h4>
			</div>	
			<div class="modal-body">
				<p id="modalInfoText"></p>
			</div>
			<div class="modal-footer">
				<button type="button" class="btn btn-default" data-dismiss="modal">Close</button>        
			</div>
		</div>
	</div>
</div>

<hr>

<script>
	function showModalInfo(mHeader, mText)
	{
		$("#modalInfoHeader").text(mHeader);
		$("#modalInfoText").text(mText);
		$("#modalInfo").modal('show');
	}

	function setDebugEnabled(mX)
	{
		var url = "php/Core/content/action_setDebugEnabled.php";
		var sentData = { enabled: mX };

		$.post(url, sentData, 
			function(mOut, mTS, mJQXHR)
			{
				
			});
	}

	function refreshDebugLo()
	{
		var url = "php/Core/content/action_refreshDebugLo.php";
		var sentData = {};

		$.post(url, sentData, 
			function(mOut, mTS, mJQXHR)
			{
				$("#debugLo").html(mOut);
			});
	}	

	function refreshSections(mTarget, mNullRow)
	{
		var url = "php/Core/content/action_refreshSections.php";
		var sentData = { nullRow: mNullRow };

		$.post(url, sentData, 
			function(mOut, mTS, mJQXHR)
			{
				$(mTarget).html(mOut);				
			});
	}

	function createSection()
	{
		var url = "php/Core/content/action_createSection.php";
		var sentData = { idParent: $("#slParent").val(), name: $("#tbName").val() };

		$.post(url, sentData,			
			function(mOut, mTS, mJQXHR)
			{
			   showModalInfo("Create", mOut);
			}
			).fail(function(mJQXHR, mTS, mErr)
			{
				showModalInfo("Error", mErr);
			});
	}

	function refreshAll()
	{
		refreshSections("#slParent", true); 
		refreshSections("#slToDel", false); 
		refreshDebugLo(); 
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
		showModalInfo("Info", "Section created successful.");
	});

	$("#btnDebugEnable").click(function()
	{ 
		setDebugEnabled(true);
		refreshDebugLo();
		showModalInfo("Info", "Debug mode enabled.");
	});
	$("#btnDebugDisable").click(function()
	{ 
		setDebugEnabled(false);
		refreshDebugLo();
		showModalInfo("Info", "Debug mode disabled.");
	});
</script>