// librairies serveurs
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
#include <pthread.h>
#include <time.h>

void *gestion_connect(void *psocket_client);

int main() {
    //mise en place du seveur
    int socket_serveur = socket(AF_INET, SOCK_STREAM, 0);

    // mise en place de l'adresse
    struct sockaddr_in adr_serv;
    adr_serv.sin_family = AF_INET;
    adr_serv.sin_port = htons(2569);
    adr_serv.sin_addr.s_addr = INADDR_ANY;

    // connecter le socket a l'ip et le port
    bind(socket_serveur, (struct sockaddr *) &adr_serv, sizeof(adr_serv));

    listen(socket_serveur, 100);

    int socket_client;
    //gérer l'arriver des clients
    while ((socket_client = accept(socket_serveur, NULL, NULL))) {
        pthread_t th_client;
        pthread_create(&th_client, NULL, gestion_connect, &socket_client);
    }
    close(socket_serveur);

    return 0;
}

void *gestion_connect(void *psocket_client) {
    //récupération socket client
    int socket_client = *((int *) psocket_client);
    char mess_client[256];
    printf("Le client est bien connecté \n");

    read(socket_client, mess_client, 256);
    printf("%s ", mess_client);
}
