<?

     include 'include/connessione.php';
	
	$result= mysql_query ("SELECT cc.ID as ccID ,po.NOME as poNOme,po.COGNOME  as poCOGNOME,p.NOME as pNOme,p.COGNOME as pCOGNOME,re.NOME_REPARTO as reNOMEREPARTO FROM CARTELLA_CLINICA as cc,PERSONALE_OSPEDALIERO as po, PAZIENTI as p, REPARTI as re WHERE po.ID=cc.ID_DOTTORE and p.ID=cc.ID_PAZIENTE and re.ID=cc.REPARTO_RICOVERO");
	$i=0;
?> 

<table class="table table-striped table-bordered"> 
<tr> 
<th id=\"ID\"></th> 
<th id=\"Nome\"> Pazienti</th> 
<th id=\"Cognome\"> </th> 
<th id=\"Nome\"> Dottore</th> 
<th id=\"Cognome\"> </th> 
<th id=\"NOme Reparto\"> Reparto</th> 
 


</tr> 

<?
	while($row=mysql_fetch_array($result))
	{

		echo "<tr><td class=tabval style=\"width:1%\"><span class=\"btn btn-xs btn-primary glyphicon glyphicon-share\" onclick=\"mostra_dottori_turni()\"></span></td>"; // AL POSTO DI MOSTRA DOTTORI TURNI DEVE MOSTRARE LA CARTELLA CLINICA DEL PAZIENTE 
		echo "<td class=tabval><b>".htmlspecialchars($row['poNOme'])."&nbsp;</td>";
		echo "<td class=tabval><b>".htmlspecialchars($row['poCOGNOME'])."&nbsp;</td>";
		echo "<td class=tabval><b>".htmlspecialchars($row['pNOme'])."&nbsp;</td>";
		echo "<td class=tabval><b>".htmlspecialchars($row['pCOGNOME'])."&nbsp;</td>";
		echo "<td class=tabval><b>".htmlspecialchars($row['reNOMEREPARTO'])."&nbsp;</td></tr>";
		
				
		
		
		$i++;
	}
?>	
</table>









	