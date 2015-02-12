<?

     include 'include/connessione.php';
	
	$result= mysql_query ("SELECT *  FROM PAZIENTI ORDER BY eta");
	$i=0;
?> 


<table class="table table-striped table-bordered"> 
<tr> 
<th id=\"id\"> ID </th>
<th id=\"nome\"> Nome </th> 
<th id=\"cognome\"> Cognome </th> 
<th id=\"data di nascita\"> Codife fiscale</th> 
<th id=\"codice fiscale\"> Data di nascita</th> 
<th id=\"eta\"> Eta </th> 
 

</tr> 

<?
	
	
	while($row=mysql_fetch_array($result))
	{

		echo "<tr><td class=tabval><b>".htmlspecialchars($row['ID'])."</b></td>";
		echo "<td class=tabval>".htmlspecialchars($row['NOME'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['COGNOME'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['CODICE_FISCALE'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['DATA_DI_NASCITA'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['ETA'])."&nbsp;</td></tr>";
	
	

		$i++;
	}
?>	
</table>


<?
	mysql_close($connessione);
?>





	