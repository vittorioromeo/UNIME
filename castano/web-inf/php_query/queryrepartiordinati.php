<?

     include 'include/connessione.php';
	
	$result= mysql_query ("SELECT r.ID as rID,r.NOME_REPARTO as rNOME_REPARTO,p.DENOMINAZIONE as pDENOMINAZIONE,ov.ORARIO_INIZIO as ovORARIOINIZIO,ov.ORARIO_FINE as ovORARIO_FINE FROM REPARTI as r, PLESSO as p, ORARI_VISITE as ov WHERE r.ID_PLESSO=p.ID_PLESSO and r.ID_ORARIO_VISITA=ov.ID ORDER BY NOME_REPARTO");
	$i=0;
?> 


 

<table class="table table-striped table-bordered"> 
<tr> 
<th id=\"ID\"> ID  </th> 
<th id=\"NOME_REPARTO\"> Nome reparto <span style="float:right"class="btn btn-primary btn-xs glyphicon glyphicon-asterisk" onclick="mostra_personale()"></span></th> 
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









	