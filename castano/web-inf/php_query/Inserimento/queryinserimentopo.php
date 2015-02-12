<?

    include 'include/connessione.php';
    
	if (isset($_GET['idTurno'])) {
        $idTurno=$_GET['idTurno'];
    }else{
        $idTurno=-1;
    }
    

	$result= mysql_query ("SELECT po.ID as poID, po.NOME as poNOME,po.COGNOME as poCOGNOME 
		                  FROM PERSONALE_OSPEDALIERO as po, ASSEGNA_TURNI as ast, TURNI as tr 
		                  WHERE ast.ID_PERSONALE_OSPEDALIERO=po.ID and ast.ID_TURNI=tr.ID and tr.ID=".$idTurno);
	$i=0;
?>
<h4>
<?if($idTurno==1){
   echo "Dettagli mattino";
}else if($idTurno==2){
   echo"Dettagli pomeriggio";
}else {
	echo "Dettagli notte";
}?>
</h4>
<table class="table table-striped table-bordered"> 
<tr> 
<th id=\"Id\"> ID</th> 
<th id=\"Nome\"> Nome </th> 
<th id=\"Cognome\"> Cognome</th> 

 


</tr> 

<?
	while($row=mysql_fetch_array($result))
	{

		
		echo "<tr><td class=tabval><b>".htmlspecialchars($row['poID'])."&nbsp;</td>";
		echo "<td class=tabval><b>".htmlspecialchars($row['poNOME'])."&nbsp;</td>";
		echo "<td class=tabval><b>".htmlspecialchars($row['poCOGNOME'])."&nbsp;</td></tr>";
		
				
		
		
		$i++;
	}
?>	
</table>









	