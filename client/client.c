// librairies client
#ifdef __WIN32__
#include <winsock2.h>
#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// Déclaration des fonctions
void vide_mess_stocker();


int main() {
    // mise ne place des variable
    char mess_stock_serv[256];
    char mess_stock_cli[256];
    char messClient;

    // mise en place du seveur
    int socket_serveur = socket(AF_INET, SOCK_STREAM, 0);

    // mise en place de l'adresse
    struct sockaddr_in adr_serv;
    adr_serv.sin_family = AF_INET;
    adr_serv.sin_port = htons(2569);
    adr_serv.sin_addr.s_addr = INADDR_ANY;

    system("clear");

    // initier connexion
    int etat_connect = connect(socket_serveur, (struct sockaddr *) &adr_serv, sizeof(adr_serv));
    // vérifier connexion
    if (etat_connect == -1) {
        printf("Erreur de connexion\n");
    }
    printf("Vous êtes bien connecté\n");

    vide_mess_stocker();

    printf("-----------------------------------\n");
    printf("|         MENU DES CHOIX          |\n");
    printf("|---------------------------------|\n");
    printf("|   1 - Faire une réservation     |\n");
    printf("|   2 - Annuler une réservation   |\n");
    printf("|   3 - Quitter l'application     |\n");
    printf("-----------------------------------\n");

    printf("\nVeuillez faire votre choix (1/2/3) : \n");
    scanf("%c", &mess_cli);

    system("clear");

    if (mess_cli == '1') {
        write(socket_serveur, "r\0", 2);
        read(socket_serveur, mess_stock_serv, sizeof(mess_stock_serv));

        printf("%s\n\n", mess_stock_serv);

        if (messStock[0] == 'U') {
            printf("---------------------------------\n");
            printf("|     Faire une réservation     |\n");
            printf("---------------------------------\n");

            vide_mess_stocker();

            printf("\nVeuillez saisir votre nom :\n");
            scanf("%s", mess_stock_cli);
            write(socket_serveur, mess_stock_cli, sizeof(mess_stock_cli)); //On envoie le nom au serveur

            vide_mess_stocker();

            printf("\nVeuillez saisir votre prénom :\n");
            scanf("%s", mess_stock_cli);
            write(socket_serveur, mess_stock_cli, sizeof(mess_stock_cli)); //On envoie le prenom au serveur

            read(socket_serveur, mess_stock_serv, sizeof(mess_stock_serv)); // On récupère le numéro de dossier
            printf("\nVotre numéro de dossier est : %s\n", mess_stock_serv);
            printf("\nVotre réservation est une réussite\n");
        }
    }
    else if (mess_cli == '2') {
        write(socket_serveur, "a\0", 2);

        printf("-----------------------------------\n");
        printf("|     Annuler une réservation     |\n");
        printf("-----------------------------------\n");

        vide_mess_stocker();

        printf("\nVeuillez saisir votre numéro de dossier à annuler :\n");
        scanf('%s', mess_stock_cli);
        write(socket_serveur, mess_stock_cli, sizeof(mess_stock_cli)); //On envoie le numéro de dossier au serveur

        read(socket_serveur, mess_stock_serv, sizeof(mess_stock_serv)); // Le serveur confirme ou non la demande
        printf("\n%s", mess_stock_serv);
    }
    else if (mess_cli == '3') {
        printf("--------------------------------\n");
        printf("|     Bonne journée à vous     |\n");
        printf("--------------------------------\n");

        vide_mess_stocker();

        printf("\n\nPressez sur la touche \"entrée\" pour pouvoir quitter l'application...\n\n");
        getchar(); // Attente que la touche "entrée" pour quité.

        system("clear");
    }

    //Fermeture de la connexion
    shutdown(socket_serveur, 2);
    close(socket_serveur);
    exit(0);

    return 0;
}


void vide_mess_stocker() {
    int c;
    while ((c = getchar()) != EOF && c != '\n');
}