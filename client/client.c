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


// Déclaration des fonctions //
void vide_mess_stocker();


int main() {
    // Mise ne place des variables
    char mess_stock_serv[256];
    char mess_stock_client[256];
    char mess_client;

    // Mise en place du serveur
    int socket_serveur = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_serveur < 0) {
        printf("Erreur de socket\n");
        exit(0);
    }

    // Mise en place de l'adresse
    struct sockaddr_in adr_serv;
    adr_serv.sin_family = AF_INET;
    adr_serv.sin_port = htons(2569);
    adr_serv.sin_addr.s_addr = INADDR_ANY;

    system("clear"); // Pour vider la page

    // Initier connexion
    int etat_connect = connect(socket_serveur, (struct sockaddr *) &adr_serv, sizeof(adr_serv));
    // Vérifier connexion
    if (etat_connect == -1) {
        printf("Erreur de connexion\n");
        exit(0);
    }
    printf("Vous êtes bien connecté\n");

    system("clear"); // Pour vider la page

    printf("-----------------------------------\n");
    printf("|         MENU DES CHOIX          |\n");
    printf("|---------------------------------|\n");
    printf("|   1 - Faire une réservation     |\n");
    printf("|   2 - Annuler une réservation   |\n");
    printf("|   3 - Quitter l'application     |\n");
    printf("-----------------------------------\n");

    do {
        printf("\nVeuillez faire votre choix (1/2/3) : \n");
        scanf("%c", &mess_client);
    } while (!((mess_client == '1') || (mess_client == '2') || (mess_client == '3')));


    system("clear"); // Pour vider la page

    if (mess_client == '1') {
        write(socket_serveur, "r\0", 2);
        read(socket_serveur, mess_stock_serv, sizeof(mess_stock_serv));

        printf("%s\n\n", mess_stock_serv);

        if (mess_stock_serv[0] == 'R') { // On regarde la premier lettre du message
            printf("---------------------------------\n");
            printf("|     Faire une réservation     |\n");
            printf("---------------------------------\n");

            printf("\nVeuillez saisir votre nom :\n");
            vide_mess_stocker();
            scanf("%s", mess_stock_client);
            write(socket_serveur, mess_stock_client, sizeof(mess_stock_client)); // On envoie le nom au serveur

            printf("\nVeuillez saisir votre prénom :\n");
            vide_mess_stocker();
            scanf("%s", mess_stock_client);
            write(socket_serveur, mess_stock_client, sizeof(mess_stock_client)); // On envoie le prénom au serveur

            read(socket_serveur, mess_stock_serv, sizeof(mess_stock_serv)); // On récupère le numéro de dossier
            printf("\nVotre numéro de dossier est : %s\n", mess_stock_serv);
            printf("\nVotre réservation est une réussite\n");
        }
    }
    else if (mess_client == '2') {
        write(socket_serveur, "a\0", 2);

        printf("-----------------------------------\n");
        printf("|     Annuler une réservation     |\n");
        printf("-----------------------------------\n");

        printf("\nVeuillez saisir votre numéro de dossier :\n");
        vide_mess_stocker();
        scanf("%s", mess_stock_client);
        write(socket_serveur, mess_stock_client, sizeof(mess_stock_client)); // On envoie le numéro de dossier au serveur

        read(socket_serveur, mess_stock_serv, sizeof(mess_stock_serv)); // Le serveur confirme ou non la demande
        printf("\n%s", mess_stock_serv);
    }
    else if (mess_client == '3') {
        printf("--------------------------------\n");
        printf("|     Bonne journée à vous     |\n");
        printf("--------------------------------\n");

        vide_mess_stocker();

        printf("\n\nPressez sur la touche \"entrée\" pour pouvoir quitter l'application...\n\n");
        getchar(); // Attente que la touche "entrée" pour quitter.

        system("clear"); // Pour vider la page
    }

    //Fermeture de la connexion
    shutdown(socket_serveur, 2);
    close(socket_serveur);
    exit(0);

    return 0;
}


// Les autres fonctions //

void vide_mess_stocker() {
    int c;
    while ((c = getchar()) != EOF && c != '\n');
}
