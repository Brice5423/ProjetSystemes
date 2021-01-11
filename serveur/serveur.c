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


// Déclaration des constante //
#define NB_MAX_PLACE 100 // Nombre de réservations max possible


// Déclaration des structure et des types //
// Mise en place du type T_Dossier
struct dossier {
    int disponible;
    char *num_dossier;
    char *nom;
    char *prenom;
};
typedef struct dossier T_Dossier;

// Mise en place du type T_Info
struct info {
    int client_socket;
    T_Dossier *ensemble_dossiers;
};
typedef struct info T_Info;


// Déclaration des fonctions //
void *gestion_connect(void *psocket_client);
void sauvegarde_des_dossiers(T_Dossier *dossier);


int main() {
    system("clear"); // Pour vider la page

    // mise en place du seveur //
    int socket_serveur = socket(AF_INET, SOCK_STREAM, 0);

    // mise en place de l'adresse //
    struct sockaddr_in adr_serv;
    adr_serv.sin_family = AF_INET;
    adr_serv.sin_port = htons(2569); // numero du port serveur
    adr_serv.sin_addr.s_addr = INADDR_ANY;

    // connecter le socket a l'ip et le port //
    bind(socket_serveur, (struct sockaddr *) &adr_serv, sizeof(adr_serv));

    // nombre de connexion simultaner //
    listen(socket_serveur, NB_MAX_PLACE);

    // gérer l'arriver des clients //
    int socket_client;
    while ((socket_client = accept(socket_serveur, NULL, NULL))) {
        pthread_t th_client;
        pthread_create(&th_client, NULL, gestion_connect, &socket_client);
    }
    close(socket_serveur);

    return 0;
}


// Les autres fonctions //

void *gestion_connect(void *psocket_client) {
    // récupération socket client
    int socket_client = *((int *) psocket_client);
    char mess_client[256];

    printf("Le client est bien connecté \n");

    read(socket_client, mess_client, 256);
    printf("%s ", mess_client);
}

void sauvegarde_des_dossiers(T_Dossier *dossier) {
    int i;
    FILE *fichier;
    fichier = fopen("sauvegarde.txt", "w"); //Ouvre un fichier en mode "écriture"

    for (i = 0; i < NB_MAX_PLACE; i++) {
        if (!dossier[i].disponible) {
            // insert la numero de dossier dans le fichier
            fputs(dossier[i].num_dossier, fichier);
            fputc('\n', fichier);

            fputs(dossier[i].nom, fichier);
            fputc('\n', fichier);

            fputs(dossier[i].prenom, fichier);
            fputc('\n', fichier);
        }
    }

    fclose(fichier); //ferme le fichier
}