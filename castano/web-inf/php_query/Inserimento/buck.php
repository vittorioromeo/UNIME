<!DOCTYPE html>

<html lang='it'>
 <head>
	<title>Progetto Base di Dati</title>
  	<link href="dischi.css" rel="stylesheet" type="text/css"> 
  </head>

  <body>
	<br><br>
		<?php include ("connessione.php");
				$Nome_Dipendente = $_POST['NomeDipendente'];
				$Cognome_Dipendente = $_POST['CognomeDipendente'];
				$Codice_fiscale = $_POST['CodiceFiscale']
				$Data_di_nascita = $_POST['DataDiNascita'];
				$Recapito  = $_POST['Recapito'];
				$Indirizzo = $_POST['Indirizzo'];
				$Stipendio = $_POST['Stipendio'];
				

		$sql	=	"INSERT INTO PERSONALE_OSPEDALIERO (NOME,COGNOME,CODICE_FISCALE,DATA_DI_NASCITA,RECAPITO,INDIRIZZO,STIPENDIO,ID_RUOLO,ID_REPARTO)
					VALUES(
						#'$Nome_Artista',
						\"$NOME\",
						\"$COGNOME\",
						\"$CODICE_FISCALE\",
						\"$DATA_DI_NASCITA\",
						\"$RECAPITO\",
						\"$INDIRIZZO\",
						\"$STIPENDIO\",
						\"$ID_RUOLO\",
						\"$ID_REPARTO\")";

		if (mysql_query($sql))
		    echo "<center>Inserimento Dipendente eseguito con successo ";
		else
		    echo "<center>Error: {$sql} <br>" . mysql_error($conn);
		}
		mysql_close($conn);
		?>
		
		<br><br>
		<table>
		    <tr><td>
			  <input class="btn" type="submit" value='Aggiungi'> <input class="btn" type="reset" submit value='Annulla'> <input class="btn" type="button" value='Indietro' onclick="mostra_personale'">
			</td></tr>
		</table>
</body>

</html>