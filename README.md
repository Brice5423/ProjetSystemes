# FONCTIONNEMENT DE L'APPLICATION

## Lancement de l'application 

- gcc client.c -o client | gcc serveur.c -o serveur (compilation)
- ./ client | ./serveur (execution)

## Début de l'application 

- Quand on lance l'application si le client ne peut pas se connecter, il y aura une erreur.
- Si le client peut se connecter, il aura alors la possibilité de choisir entre 3 options :

## Faire une réservation
- Si le client fait une réservation, et qu'il y a encore de la place, il doit entrer son nom, prénom. Ainsi, il obtiendra son numéro de dossier et un message pour dire que la réservation est un succès.
 
## Annuler une réservation
 - Si le client veut annuler sa réservation, il doit entrer son numéro de dossier, s'il est valide.

## Quitter l'application
- Si le client quitte l'application, il reçoit alors un message pour montrer qu'il quitte bien l'application

--------------------------------------------------------------------------------

# Pour nous (Brice.O & Nicolas.C)
## Lien GitHub
https://github.com/Brice5423/ProjetSystemes

### Commande de base
- **cd**
- **ls**

### Commande pour le code
- **gcc nom_fichier.c –o nom_executable** : Compilation
- **./nom_executable test_affichage1 test_affichage2** : Exécution
