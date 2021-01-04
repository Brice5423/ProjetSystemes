// librairies client
#ifdef __WIN32__
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    //mise en place du seveur
    int socket_serveur = socket (AF_INET,SOCK_STREAM,0);

    // mise en place de l'adresse
    struct sockaddr_in adr_serv;
    adr_serv.sin_family = AF_INET;
    adr_serv.sin_port = htons(2569);
    adr_serv.sin_addr.s_addr = INADDR_ANY;

    //initier connexion
    int etat_connect = connect (socket_serveur, (struct sockaddr *) &adr_serv, sizeof(adr_serv));
    //vérifier connexion
    if (etat_connect == -1) {
        printf("Erreur de connexion \n");
    }
    char mess_serveur [256];
    char* stock_mess_cl;
    printf("Vous êtes bien connecté \n");
    scanf("%s ", stock_mess_cl);

    write(socket_serveur, "test", 8);

    return 0;
}

