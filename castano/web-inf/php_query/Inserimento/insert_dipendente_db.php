<!DOCTYPE html>
<html>
 
 
 <head >
 <title>Progetto Base di Dati</title>
</head>

  <body>
	<br><br>
		<?php include ("connessione.php");
		
				
				$Nome_Dipendente = $_POST['NomeDipendente'];
				$Cognome_Dipendente = $_POST['CognomeDipendente'];
				$Codice_fiscale = $_POST['CodiceFiscale'];
				$Data_di_nascita = $_POST['DataDiNascita'];
				$Recapito  = $_POST['Recapito'];
				$Indirizzo = $_POST['Indirizzo'];
				$Stipendio = $_POST['Stipendio'];
				$ID_RUOLO = $_POST['Ruoli'];
				$ID_REPARTO = 1;
				

		$sql	=	"INSERT INTO PERSONALE_OSPEDALIERO (NOME,COGNOME,CODICE_FISCALE,DATA_DI_NASCITA,RECAPITO,INDIRIZZO,STIPENDIO,ID_RUOLO,ID_REPARTO)
					VALUES(
						\"$Nome_Dipendente\",
						\"$Cognome_Dipendente\",
						\"$Codice_fiscale\",
						\"$Data_di_nascita\",
						\"$Recapito\",
						\"$Indirizzo\",
						\"$Stipendio\",
						\"$ID_RUOLO\",
						\"$ID_REPARTO\")";

		if (mysql_query($sql))
		    echo "<center>Inserimento Dipendente eseguito con successo ";
		else
		    echo "<center>Error: {$sql} <br>" . mysql_error($connessione);
		mysql_close($connessione);
		?>
		 

</body>
</html>