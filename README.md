# Lien GitHub
https://github.com/Brice5423/ProjetSystemes

# Commande terminal

## Commande de base
- **cd**
- **ls**


## Commande pour le code
- **gcc nom_fichier.c –o nom_executable** : Compilation
- **./nom_executable test_affichage1 test_affichage2** : Exécution

--------------------------------------------------------------------------------

# Tache à faire

- C/S : établir la connexion Client/Serveur
- S : Créer un thread pour chaque client qui se connecte
- C : Choix Reserver / Annuler une réservation.

## I) Reservation
- C : Envoie à S qu'il veut réserver
- S : Reçoit le message et chercher si une place est disponible puis envoit à C sa réponse

### I.1) Aucune place n'est dispo
- C : Reçoit le message du serveur

#### FIN

### I.2) Une place est dispo
- C : Reçoit le message du serveur, puis demande à l'utilisateur de saisir son nom et l'envoie à S
- S : Reçoit le nom
- C : Demande le prénom à l'utilisateur puis l'envoie à S
- R : Reçoit le prénom, génère un numéro de dossier à 10 chiffres (un int ne suffit pas), fait la reservation et envoie à C le numéro de dossier
- C : Reçoit le numéro de dossier

#### FIN

## II) Annulation
- C : Envoie à S qu'il veut annuler
- S : Reçoit le message
- C : Demande à l'utilisateur de saisir le nom et l'envoie à S
- S : Reçoit le nom
- C : Demande à l'utilisateur de saisir le numéro de dossier et l'envoie à S
- S : Reçoit le numéro de dossier puis chercher s'il trouve l'association nom/numdossier (strcmp pour comparer des chaines), si elle existe, annule la reservation puis envoie à C s'il a ou non trouvé la reservation
- C : Reçoit le message

###FIN

Pour les dossiers, je créerait une structure avec tous les champs nécessaires d'un dossier (penser à un champ pour savoir si le dossier est dispo ou pas) et pour les arguments du thread, je ferais une autre structure avec le socketclient et un tableau de 100 dossier (car dans le sujet, il y a 100 places)
Des questions ? x)

# Lien code

"1ère variable la nôtre et 2ème celle de Hale"
    Dans serveur.c
    
- socket_server -> ma_socket
- sockaddr_in -> sockaddr_in
- adr_serv -> my_adr
- socket_client -> client_socket
- mess_client ->
- psocket_client ->

    Dans client.c
    
- socket_serveur -> to_servers_socket
- adr_serv -> server_socket_address
- sockaddr_in -> sockaddr_in
- etat_connect -> 
