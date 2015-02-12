<?

     include 'include/connessione.php';
	
	$result= mysql_query ("INSERT INTO PERSONALE_OSPEDALIERO (NOME, COGNOME, CODICE_FISCALE, DATA_DI_NASCITA, RECAPITO, INDIRIZZO,STIPENDIO,ID_RUOLO,ID_REPARTO)");
	$i=0;
?> 


 

<table class="table table-striped table-bordered"> 
<tr> 
<th id=\"ID\"> ID </th> 
<th id=\"NOME_REPARTO\"> Nome reparto</th> 
<th id=\"ID_PLESSO\"> Plesso</th> 
<th id=\"POSTI_LETTO\"> Inizio visita</th> 
<th id=\"ID_ORARIO_VISITA\"> Fine visita </th> 

</tr> 



<?
	while($row=mysql_fetch_array($result))
	{

		echo "<tr><td class=tabval><b>".htmlspecialchars($row['rID'])."</b></td>";
		echo "<td class=tabval>".htmlspecialchars($row['rNOME_REPARTO'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['pDENOMINAZIONE'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['ovORARIOINIZIO'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['ovORARIO_FINE'])."&nbsp;</td></tr>";
		
		
		
		$i++;
	}
?>	
</table>








	