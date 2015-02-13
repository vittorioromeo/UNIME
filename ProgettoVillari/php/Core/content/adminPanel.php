

<div class="container">
	<h1>Administration panel</h1>
	<div class="row">
		<div class="col-md-4">
			<h2>Sections</h2>
			<?php Gen::BSPanelStart("Create"); ?>
			<form name="formAddSection" id="formAddSection" action="action_createSection.php">
				
				<button type="button" class="btn btn-default" id="btnRefresh">
					<span class="glyphicon glyphicon-refresh" aria-hidden="true"></span>
				</button>

				<?php
					Gen::Textbox("tbName", "Section name");
				?>

				<div class="form-group">
						<label for="slParent">Parent section</label>
					<select class="form-control" name="slParent" id="slParent" onchange="showUser(this.value)">
						<option value="-1">None</option>

						<?php
							foreach(Tables::$section->getAllRows() as $x)
							{
								print("<option value=".$x["id"].">(".$x["id"].") ".$x["name"]."</option>");
							}
						?>

						<option value="1">Peter Griffin</option>
						<option value="2">Lois Griffin</option>
						<option value="3">Joseph Swanson</option>
						<option value="4">Glenn Quagmire</option>
					</select>
				</div>		 
			</form>
			<?php Gen::BSPanelEnd(); ?>
		</div>
		<div class="col-md-4">
			<h2>Heading</h2>
			<p>Donec id elit non mi porta gravida at eget metus. Fusce dapibus, tellus ac cursus commodo, tortor mauris condimentum nibh, ut fermentum massa justo sit amet risus. Etiam porta sem malesuada magna mollis euismod. Donec sed odio dui. </p>
			<p><a class="btn btn-default" href="#" role="button">View details &raquo;</a></p>
	 </div>
		<div class="col-md-4">
			<h2>Heading</h2>
			<p>Donec sed odio dui. Cras justo odio, dapibus ac facilisis in, egestas eget quam. Vestibulum id ligula porta felis euismod semper. Fusce dapibus, tellus ac cursus commodo, tortor mauris condimentum nibh, ut fermentum massa justo sit amet risus.</p>
			<p><a class="btn btn-default" href="#" role="button">View details &raquo;</a></p>
		</div>
	</div>

	<hr>

	<footer>
		<p>&copy; Company 2015</p>
	</footer>
</div>         

<script>

	$("#btnRefresh").click(function() 
	{
		var idParent = $("#slParent").val();
		var name = $("#tbName").val();

		var URL = "php/Core/content/" + $("#formAddSection").attr("action");
		var formData = 
		{
			idParent: idParent,
			name: name
		};
//$("#debugLo").clear();
		$("#debugLo").text("DIO BELLO");

		$.post(URL,
		    formData,
		    function(data, textStatus, jqXHR)
		    {
		       alert(data);
		    }).fail(function(jqXHR, textStatus, errorThrown) 
		    {
		 		alert(errorThrown);
		    });


	});
</script>