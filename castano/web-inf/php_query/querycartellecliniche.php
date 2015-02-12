<?

     include 'include/connessione.php';
	
	$result= mysql_query ("SELECT cc.ID as ccID,p.NOME as pNOME,p.COGNOME as pCOGNOME,po.NOME as poNOME ,po.COGNOME as poCOGNOME,cc.DATA_RICOVERO as ccDATA_RICOVERO,cc.DATA_DIMISSIONE as ccDATA_DIMISSIONE,cc.POSTO_LETTO as ccPOSTO_LETTO,cc.SINTOMI as ccSINTOMI,cc.DIAGNOSI as ccDIAGNOSI FROM CARTELLA_CLINICA as cc, PAZIENTI as p, PERSONALE_OSPEDALIERO as po WHERE p.ID=cc.ID_PAZIENTE and po.ID=cc.ID_DOTTORE");
	$i=0;
?> 


<table class="table table-striped table-bordered"> 
<tr> 
<th id=\"ID\"> ID </th>
<th id=\"nome\">  Paziente  </th> 
<th id=\"cognome\">  </th> 
<th id=\"cognome\"> Dottore  </th> 
<th id=\"cognome\">  </th> 
<th id=\"data di nascita\"> Data ricovero</th> 
<th id=\"codice fiscale\"> Data dimissione</th> 
<th id=\"Posto letto\"> Posto letto</th> 
<th id=\"Sintomi\"> Sintomi</th> 
<th id=\"Diagnosi\"> Diagnosi</th> 
 </tr> 

<?
	
	
	while($row=mysql_fetch_array($result))
	{

		echo "<tr><td class=tabval><b>".htmlspecialchars($row['ccID'])."</b></td>";
		echo "<td class=tabval>".htmlspecialchars($row['pNOME'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['pCOGNOME'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['poNOME'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['poCOGNOME'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['ccDATA_RICOVERO'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['ccDATA_DIMISSIONE'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['ccPOSTO_LETTO'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['ccSINTOMI'])."&nbsp;</td>";
		echo "<td class=tabval>".htmlspecialchars($row['ccDIAGNOSI'])."&nbsp;</td></tr>";
		$i++;
	}
?>	
</table>


<?
	mysql_close($connessione);
?>