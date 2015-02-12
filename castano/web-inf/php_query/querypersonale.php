<?

     include 'include/connessione.php';
	
	$result= mysql_query ("SELECT po.ID as poID ,po.NOME as poNOME ,po.COGNOME as poCOGNOME,po.CODICE_FISCALE as poCODICE_FISCALE,po.DATA_DI_NASCITA as poDATA_DI_NASCITA,po.RECAPITO as poRECAPITO,po.INDIRIZZO poINDIRIZZO,po.STIPENDIO as poSTIPENDIO,r.TIPO as rTIPO,re.NOME_REPARTO as reNOME_REPARTO FROM PERSONALE_OSPEDALIERO as po, RUOLI as r, REPARTI as re WHERE po.ID_RUOLO=r.ID and po.ID_REPARTO=re.ID ORDER BY po.ID ASC");
	$i=0;
?> 

<div id="posizionepulsanti">
<h6 style="display:inline;padding:10px;">Operazioni</h6>
<span class=" btn btn-primary btn-xs glyphicon glyphicon-plus" _style="font-size:5px" onclick="inserisci_dottore()"> </span>
<span class=" btn btn-primary btn-xs glyphicon glyphicon-remove" _style="font-size:5px" onclick="elimina_dottore()"> </span>
</div>

<div id="posizionetabella">
<table class="table table-striped table-bordered"> 
<tr> 
<th id=\"nome\"> ID </th> 
<th id=\"nome\"> Nome </th> 
<th id=\"cognome\"> Cognome </th> 
<th id=\"codice fiscale\"> Codife fiscale</th> 
<th id=\"data di nascita\"> Data di nascita</th> 
<th id=\"recapito\"> Recapito </th> 
<th id=\"stipendio\"> Stipendio</th> 
<th id=\"Professione\"> Professione </th> 
<th id=\"Reparto\"> Reparto</th> 

</tr> 

<?
	
	
	while($row=mysql_fetch_array($result))
	{

		echo "<tr><td class=tabval><b>".htmlspecialchars($row['poID'])."</b></td>";
		echo "<td class=tabval>".htmlspecialchars($row['poNOME'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['poCOGNOME'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['poCODICE_FISCALE'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['poDATA_DI_NASCITA'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['poRECAPITO'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['poSTIPENDIO'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['rTIPO'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['reNOME_REPARTO'])."&nbsp;</td></tr>";
	

		$i++;
	}
?>	

</table>
</div>


<?
	mysql_close($connessione);
?>





	