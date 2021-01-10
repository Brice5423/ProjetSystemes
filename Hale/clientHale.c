//
// Created by brice on 08/01/2021.
//

#include "clientHale.h"


int to_servers_socket = -1;


void vider_buffer() {
    int c;
    while ((c = getchar()) != EOF && c != '\n');
}


int main() {
    struct sockaddr_in server_socket_address;
    struct hostent *server_host_ent;
    long hostaddr;
    char buffer[512];
    char buffer2[512];

    char c;

    char server_name[16];

    printf("Saisir l'IP du serveur :\n");
    scanf("%s", server_name); //On récupère l'IP du serveur

    bzero(&server_socket_address, sizeof(server_socket_address));
    hostaddr = inet_addr(server_name);

    if (hostaddr != (long) -1)
        bcopy(&hostaddr, &server_socket_address.sin_addr, sizeof(hostaddr));
    else {
        server_host_ent = gethostbyname(server_name);
        if (server_host_ent == NULL) {
            printf("Problème au niveau de l'hôte\n");
            exit(0);
        }
        bcopy(server_host_ent->h_addr, &server_socket_address.sin_addr, server_host_ent->h_length);
    }

    server_socket_address.sin_port = htons(30000);
    server_socket_address.sin_family = AF_INET;

    if ((to_servers_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) { //Création de la socket
        printf("Création socket client ratée !\n");
        exit(0);
    }

    if (connect(to_servers_socket, (struct sockaddr *) &server_socket_address,
                sizeof(server_socket_address)) < 0) {  //Demande de connexion
        printf("Demande de connexion ratée\n");
        exit(0);
    }

    printf("Voulez-vous Réserver ou Annuler ? (R/A)\n"); //TODO MENU

    vider_buffer();
    scanf("%c", &c);
    if (c == 'R') { //Faire une réservation
        write(to_servers_socket, "r\0", 2); //On dit au serveur que l'on s'apprête a réserver
        read(to_servers_socket, buffer, sizeof(buffer)); //Le serveur vérifie si une place est disponible
        printf("%s\n", buffer);

        if (buffer[0] == 'U') { //Une place est disponible
            printf("Veuillez saisir votre nom :\n");
            vider_buffer();
            scanf("%s", buffer2);
            write(to_servers_socket, buffer2, sizeof(buffer2)); //On envoie le nom au serveur

            printf("Veuillez saisir votre prenom :\n");
            vider_buffer();
            scanf("%s", buffer2);
            write(to_servers_socket, buffer2, sizeof(buffer2)); //On envoie le prenom au serveur

            read(to_servers_socket, buffer, sizeof(buffer)); // On récupère le numéro de dossier
            printf("Votre numéro de dossier est : %s\n", buffer);
        }
    }
    if (c == 'A') { //Annuler un dossier
        write(to_servers_socket, "a\0", 2); //On dit au serveur que l'on annule

        printf("Veuillez saisir votre nom :\n");
        vider_buffer();
        scanf("%s", buffer2);
        write(to_servers_socket, buffer2, sizeof(buffer2)); //On envoie le nom au serveur

        printf("Veuillez saisir votre numéro de dossier :\n");
        vider_buffer();
        scanf("%s", buffer2);
        write(to_servers_socket, buffer2, sizeof(buffer2)); //On envoie le numéro au serveur
        read(to_servers_socket, buffer, sizeof(buffer)); // Le serveur confirme ou non la demande
        printf("%s\n", buffer);
    }

    shutdown(to_servers_socket, 2); //Fermeture de la connexion
    close(to_servers_socket);
}