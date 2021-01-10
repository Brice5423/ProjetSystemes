//
// Created by brice on 08/01/2021.
//

#include "clientHale.h"


int to_servers_socket = -1;


void clearScreen() {
    system("cls");      //Vide l'écran
    system("clear");    //Vide l'écran
}


//--------------------------------------------------------------------------------------------------------------


void vider_buffer() {
    int c;                                            // Fonction pour vider le buffer
    while ((c = getchar()) != EOF && c != '\n');      // Fonction pour vider le buffer
}


//--------------------------------------------------------------------------------------------------------------


int main() {
    struct sockaddr_in server_socket_address;
    struct hostent *server_host_ent;
    long hostaddr;
    char buffer[512];
    char buffer2[512];

    char c;

    char server_name[16];
    clearScreen();

    printf("Saisir l'IP du serveur :\n");
    scanf("%s", server_name); //On récupère l'IP du serveur

    bzero(&server_socket_address, sizeof(server_socket_address));
    hostaddr = inet_addr(server_name);

    if (hostaddr != (long) -1)
        bcopy(&hostaddr, &server_socket_address.sin_addr, sizeof(hostaddr));
    else {
        server_host_ent = gethostbyname(server_name);
        if (server_host_ent == NULL) {
            printf("\n");
            printf("Problème au niveau de l'hôte !\n");
            exit(0);
        }
        bcopy(server_host_ent->h_addr, &server_socket_address.sin_addr, server_host_ent->h_length);
    }

    server_socket_address.sin_port = htons(30000);
    server_socket_address.sin_family = AF_INET;

    if ((to_servers_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) { //Création de la socket
        printf("\n");
        printf("Création socket client ratée !\n");
        exit(0);
    }

    if (connect(to_servers_socket, (struct sockaddr *) &server_socket_address,
                sizeof(server_socket_address)) < 0) {  //Demande de connexion
        printf("\n");
        printf("Demande de connexion ratée !\n");
        exit(0);
    }

    clearScreen();

    printf("**************************************************************************\n");
    printf("****         ------------------------------------------------         ****\n");
    printf("****        | Application de réservation de place de concert |        ****\n");
    printf("****         ------------------------------------------------         ****\n");
    printf("****                                                                  ****\n");
    printf("****                                                                  ****\n");
    printf("****   GERARD Alexandre                                    S3 GRP2    ****\n");
    printf("****   GIANGRECO Vincent                                 2020-2021    ****\n");
    printf("****                                                                  ****\n");
    printf("**************************************************************************\n");
    printf("****                         |MENU PRINCIPAL|                         ****\n");
    printf("**************************************************************************\n");
    printf("****                                                                  ****\n");
    printf("****                1 -    Faire une réservation                      ****\n");
    printf("****                2 -   Annuler une réservation                     ****\n");
    printf("****                3 -    Quitter l'application                      ****\n");
    printf("****                                                                  ****\n");
    printf("**************************************************************************\n");

    printf("\n");
    printf("Veuillez faire votre choix (1/2/3) :\n");

    vider_buffer();
    scanf("%c", &c);

    clearScreen();

    if (c == '1') { //Faire une réservation
        write(to_servers_socket, "r\0", 2); //On dit au serveur que l'on s'apprête a réserver
        read(to_servers_socket, buffer, sizeof(buffer)); //Le serveur vérifie si une place est disponible
        printf("%s\n\n", buffer);

        if (buffer[0] == 'U') { //Une place est disponible
            printf("************************************\n");
            printf("*                                  *\n");
            printf("*    --------------------------    *\n");
            printf("*   |   Faire une réservation  |   *\n");
            printf("*    --------------------------    *\n");
            printf("*                                  *\n");
            printf("************************************\n");

            printf("\nFaire une réservation\n");
            printf("\nVeuillez saisir votre nom :\n");

            vider_buffer();
            scanf("%s", buffer2);
            write(to_servers_socket, buffer2, sizeof(buffer2)); //On envoie le nom au serveur

            printf("\nVeuillez saisir votre prenom :\n");
            vider_buffer();
            scanf("%s", buffer2);
            write(to_servers_socket, buffer2, sizeof(buffer2)); //On envoie le prenom au serveur

            read(to_servers_socket, buffer, sizeof(buffer)); // On récupère le numéro de dossier
            printf("\nVotre numéro de dossier est : %s\n", buffer);

            printf("\nVotre réservation à bel et bien été faite !\n\n");
        }
    }
    if (c == '2') { //Annuler un dossier
        printf("*************************************\n");
        printf("*                                   *\n");
        printf("*    ---------------------------    *\n");
        printf("*   |  Annuler une réservation  |   *\n");
        printf("*    ---------------------------    *\n");
        printf("*                                   *\n");
        printf("*************************************\n");

        write(to_servers_socket, "a\0", 2); //On dit au serveur que l'on annule

        printf("\nVeuillez saisir votre nom :\n");
        vider_buffer();
        scanf("%s", buffer2);
        write(to_servers_socket, buffer2, sizeof(buffer2)); //On envoie le nom au serveur

        printf("\nVeuillez saisir votre numéro de dossier :\n");
        vider_buffer();
        scanf("%s", buffer2);
        write(to_servers_socket, buffer2, sizeof(buffer2)); //On envoie le numéro au serveur
        read(to_servers_socket, buffer, sizeof(buffer)); // Le serveur confirme ou non la demande
        printf("\n%s\n\n", buffer);
    }

    if (c == '3') { //Arrêter l'application
        printf("*************************************\n");
        printf("*                                   *\n");
        printf("*    ---------------------------    *\n");
        printf("*   |   Quitter l'application   |   *\n");
        printf("*    ---------------------------    *\n");
        printf("*                                   *\n");
        printf("*************************************\n");
        vider_buffer();
        printf("\nBonne journée à vous !\n");
        printf("\n");
        printf("Pressez la touche entrée pour quitter...\n\n");
        getchar(); // Attente de touche entrée pour continuer
        clearScreen();
        shutdown(to_servers_socket, 2); //Fermeture de la connexion
        close(to_servers_socket);
        exit(0);
    }

    shutdown(to_servers_socket, 2); //Fermeture de la connexion
    close(to_servers_socket);
    exit(0);
}