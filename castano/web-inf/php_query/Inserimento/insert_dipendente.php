

<!DOCTYPE html>

 <?php include 'connessione.php';

?>

  
  
	<h3 style="margin-left:10px padding=4px">Inserisci dipendente</h3><br>
    <iframe name="iframeResponse"></iframe>
	<form method="POST" action="php_query/Inserimento/insert_dipendente_db.php" target="iframeResponse">
	
    <input required class="textbox" placeholder="Nome dipendente " type="text" name="NomeDipendente"><br>
	<input required class="textbox" placeholder="Cognome dipendente " type="text" name="CognomeDipendente"><br>
    <input required class="textbox" placeholder="Codice fiscale " type="text" name="CodiceFiscale"><br>
     <input required class="textbox" placeholder="Data di nascita " type="date" name="DataDiNascita"><br>
   <input required class="textbox" placeholder="Recapito " type="text" name="Recapito"><br>
   <input required class="textbox" placeholder="Indirizzo " type="text" name="Indirizzo"><br>
   <input required class="textbox" placeholder="Stipendio " type="text" name="Stipendio"><br>
   
    ruolo:<select style="margin-top:10px"required name="Ruoli">
        <?php
        $result= mysql_query ("SELECT * FROM RUOLI");

            while ($row=mysql_fetch_assoc($result))
            {
            $id= $row["ID"];
            $tipo = $row["TIPO"];
            echo "<option value='".$id."'>".$tipo."</option>";
            }
        ?>
        </select><br>
      </td></tr>
    

      
        <button style="margin-top:15px" class="btn btn-primary " type="submit" >Aggiungi</button>
        <span style="margin-top:15px" class="btn btn-primary "onclick="mostra_personale()">Indietro</span>
    

	</form>
  
