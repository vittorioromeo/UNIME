<?

     include 'include/connessione.php';
	
	$result= mysql_query ("SELECT PAZIENTI.nome as pnome,PAZIENTI.cognome as pcognome,PERSONALE_OSPEDALIERO.nome,PERSONALE_OSPEDALIERO.cognome,CARTELLA_CLINICA.data_ricovero,CARTELLA_CLINICA.data_dimissione,REPARTI.nome_reparto FROM CARTELLA_CLINICA , PAZIENTI, PERSONALE_OSPEDALIERO, REPARTI WHERE PAZIENTI.id=CARTELLA_CLINICA.id_paziente and PERSONALE_OSPEDALIERO.id=CARTELLA_CLINICA.id_dottore and CARTELLA_CLINICA.reparto_ricovero=REPARTI.ID");
	$i=0;
?> 


<table class="table table-striped table-bordered"> 
<tr> 
<th id=\" nome paziente \"> Paziente </th> 
<th id=\" cognome paziente\">        </th> 
<th id=\"nome dottore "> Dottore     </th> 
<th id=\"cognome dottore\">          </th> 
<th id=\"data ricovero\"> Data ricovero </th> 
<th id=\"data dimissione\"> Data dimissione</th> 
<th id=\"reparto\"> Reparto </th> 


</tr> 

<?
	
	
	while($row=mysql_fetch_array($result))
	{

		echo "<tr><td class=tabval><b>".htmlspecialchars($row['pnome'])."</b></td>";
		echo "<td class=tabval>".htmlspecialchars($row['pcognome'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['nome'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['cognome'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['data_ricovero'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['data_dimissione'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['nome_reparto'])."&nbsp;</td></tr>";
		
	

		$i++;
	}
?>	
</table>


<?
	mysql_close($connessione);
?>





	