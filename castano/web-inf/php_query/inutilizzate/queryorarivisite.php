<?

     include 'include/connessione.php';
	
	$result= mysql_query ("SELECT * FROM ORARI_VISITE");
	$i=0;
?> 

<table class="table table-striped table-bordered"> 
<tr> 
<th id=\"ID\"> ID </th> 
<th id=\"ORARIO_INIZIO\"> Orario inizio</th> 
<th id=\"ORARIO_FINE\"> Orario fine</th> 


</tr> 

<?
	while($row=mysql_fetch_array($result))
	{

		echo "<tr><td class=tabval><b>".htmlspecialchars($row['ID'])."</b></td>";
		echo "<td class=tabval><b>".htmlspecialchars($row['ORARIO_INIZIO'])."&nbsp;</td>";
		echo "<td class=tabval><b>".htmlspecialchars($row['ORARIO_FINE'])."&nbsp;</td></tr>";
				
		
		
		$i++;
	}
?>	
</table>









	