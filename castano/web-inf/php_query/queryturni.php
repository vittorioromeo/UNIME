<?

     include 'include/connessione.php';
	
	$result= mysql_query ("SELECT * FROM TURNI");
	$i=0;
?> 

<table class="left table table-striped table-bordered"> 
<tr> 
<th id=\"ID\"></th> 
<th id=\"ID\"> ID </th> 
<th id=\"DENOMINAZIONE\"> Denominazione</th> 
<th id=\"ORARIO\"> Orario</th> 


</tr> 

<?
	while($row=mysql_fetch_array($result))
	{

		echo "<tr><td class=tabval style=\"width:2%\"><span class=\"btn btn-primary glyphicon glyphicon-zoom-in\" onclick=\"mostra_dottori_turni('".$row['ID']."')\"></span></td>";
		echo "<td class=tabval><b>".htmlspecialchars($row['ID'])."</b></td>";
		echo "<td class=tabval><b>".htmlspecialchars($row['DENOMINAZIONE'])."&nbsp;</td>";
		echo "<td class=tabval><b>".htmlspecialchars($row['ORARIO'])."&nbsp;</td></tr>";
	
		$i++;
	}
?>	
</table>

<?
	mysql_close($connessione);
?>


<div id="dettaglio_turno">
</div>








	