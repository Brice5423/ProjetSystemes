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
#define DISPONIBLE 1
#define INDISPONIBLE 0

// Déclaration des structure et des types //
// Mise en place du type T_Dossier
struct s_dossier {
    int disponible;
    char *num_dossier; // pointeur pour qu'on puisse faire un tableau de char
    char *nom;
    char *prenom;
};
typedef struct s_dossier T_Dossier;

// Mise en place du type T_Info
struct s_info {
    int socket_client;
    T_Dossier *ensemble_dossiers;
};
typedef struct s_info T_Info;


// Déclaration des fonctions //
void *gestion_connect(void *psocket_client);
void sauvegarde_des_dossiers(T_Dossier *dossier);



int main() {
    system("clear"); // Pour vider la page

    srand(time(NULL));

    // mise en place du seveur //
    int socket_serveur = socket(AF_INET, SOCK_STREAM, 0);

    // mise en place de l'adresse //
    struct sockaddr_in adr_serveur;
    bzero(&adr_serveur, sizeof(adr_serveur));
    adr_serveur.sin_family = AF_INET;
    adr_serveur.sin_port = htons(2569); // numero du port serveur (2569)
    adr_serveur.sin_addr.s_addr = INADDR_ANY;

    // connecter le socket a l'ip et le port //
    bind(socket_serveur, (struct sockaddr *) &adr_serveur, sizeof(adr_serveur));

    // nombre de connexion simultaner //
    listen(socket_serveur, NB_MAX_PLACE);


    T_Dossier *liste_dossier;
    liste_dossier = (T_Dossier *) malloc(sizeof(T_Dossier) * NB_MAX_PLACE);


    FILE *fichier_r;
    fichier_r = fopen("sauvegarde.txt", "r"); // Ouvre un fichier en mode "lecture"

    int j;
    int i = 0;
    char carac_verif; // Parcourt le fichier de sauvegarde //
    while (carac_verif != EOF && (carac_verif = fgetc(fichier_r)) != EOF) {
        if (i == 0) {
            printf("---------------------------------------------\n");
            printf("|     Parcourt le fichier de sauvegarde     |\n");
            printf("---------------------------------------------\n");
        }

        liste_dossier[i].num_dossier = malloc(11);
        liste_dossier[i].nom = malloc(255);
        liste_dossier[i].prenom = malloc(255);

        liste_dossier[i].disponible = INDISPONIBLE; // = 0
        liste_dossier[i].num_dossier[0] = carac_verif;

        j = 1; // Récupère le numéro de dossier //
        while ((carac_verif = fgetc(fichier_r)) != '\n') {
            liste_dossier[i].num_dossier[j] = carac_verif;
            j++;
        }
        liste_dossier[i].num_dossier[j] = '\0';

        j = 0; // Récupère le nom de la personne //
        while ((carac_verif = fgetc(fichier_r)) != '\n') {
            liste_dossier[i].nom[j] = carac_verif;
            j++;
        }

        j = 0; // Récupère le prenom de la personne //
        while ((carac_verif = fgetc(fichier_r)) && carac_verif != '\n') {
            liste_dossier[i].prenom[j] = carac_verif;
            j++;
        }

        // affichage des dossiers en fonction de "i" //
        printf("\nDossier [%s] :\t num dos =  %s,\t nom = %s,\t prenom = %s\n",
               i+1, liste_dossier[i].num_dossier, liste_dossier[i].nom, liste_dossier[i].prenom);

        i++;
    }

    fclose(fichier_r);


    int i_reste; // Complète le reste du dossier comme DISPONIBLE //
    for (i_reste = i; i_reste < NB_MAX_PLACE; i_reste++) {
        liste_dossier[i_reste].disponible = DISPONIBLE; // = 1
        liste_dossier[i_reste].num_dossier = NULL;
        liste_dossier[i_reste].nom = NULL;
        liste_dossier[i_reste].prenom = NULL;
    }


    // Gère l'arriver des clients //
    printf("----------------------------------------\n");
    printf("|     Action liée avec les client      |\n");
    printf("----------------------------------------\n\n");
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
    int socket_client = *((int *) psocket_client); // Récupération socket client

    char mess_stock[256];
    char num_dossier[11];

    T_Info *info_client;
    info_client = (T_Info *) psocket_client;

    printf("\nLe client est bien connecté\n\n");

    int c = 0;
    int i = -1;

    read(socket_client, mess_stock, sizeof(mess_stock));
    if (mess_stock[0] == 'r') { //Faire une réservation
        while (c < NB_MAX_PLACE) {
            if (info_client->ensemble_dossiers[c].disponible) { // Chercher un dossier disponible
                info_client->ensemble_dossiers[c].disponible = 0; // Réserve dossier

                i = c;
                c = NB_MAX_PLACE;
            }
            c++;
        }

        if (i == -1) {
            write(socket_client, "Aucun réservation possible", 128);
        }
        else {
            write(socket_client, "Réservation possible", 128);
            read(socket_client, mess_stock, sizeof(mess_stock)); // Récupération du nom du client
            info_client->ensemble_dossiers[i].nom = strdup(mess_stock); // Affectation du nom du client au dossier
            read(socket_client, mess_stock, sizeof(mess_stock)); // Récupération du prénom du client
            info_client->ensemble_dossiers[i].prenom = strdup(mess_stock); // Affectation du prénom du client au dossier

            c = 0;
            while (c < 10) { // Génération aléatoire d'un numéro de dossier
                num_dossier[c] = '0' + (rand() % 10);
                c++;
            }
            num_dossier[10] = '\0';
            info_client->ensemble_dossiers[i].num_dossier = strdup(num_dossier); // Affectation du numéro de dossier
            write(socket_client, num_dossier, sizeof(num_dossier)); // Envoi du numéro de dossier
            printf("Votre dossier (n° %s) a été réservé par %s %s.\n",
                   num_dossier, info_client->ensemble_dossiers[i].nom, info_client->ensemble_dossiers[i].prenom);

            sauvegarde_des_dossiers(info_client->ensemble_dossiers);
        }
    }
    else { // Annulation d'une réservation
        read(socket_client, mess_stock, sizeof(mess_stock)); // Récupère le nom du client
        read(socket_client, num_dossier, sizeof(num_dossier)); // Récupère le numéro du dossier

        i = 0;

        while (c < NB_MAX_PLACE) { // Parcours des dossiers
            if (!info_client->ensemble_dossiers[c].disponible) {
                // Verification du bon dossier à supprimée //
                if (!strcmp(num_dossier, info_client->ensemble_dossiers[c].num_dossier)) {
                    info_client->ensemble_dossiers[c].disponible = DISPONIBLE; // = 1
                    info_client->ensemble_dossiers[c].nom = NULL;
                    info_client->ensemble_dossiers[c].prenom = NULL;
                    info_client->ensemble_dossiers[c].num_dossier = NULL;

                    sauvegarde_des_dossiers(info_client->ensemble_dossiers); // Mettre à jour le fichier sauvegarde

                    write(socket_client, "Réservation a été annulée avec succès", 128);
                    printf("Votre dossier (n° %s) a été annulé avec succès\n", num_dossier);

                    c = NB_MAX_PLACE;
                    i = 1;
                }
            }
            c++;
        }

        if (!i) { // La reservation n'existe pas
            write(socket_client, "Votre réservation est introuvable", 128);
        }
    }

    shutdown(socket_client, 2); // pause
    close(socket_client); // Déconnexion
}

void sauvegarde_des_dossiers(T_Dossier *dossier) {
    int i;
    FILE *fichier_w;
    fichier_w = fopen("sauvegarde.txt", "w"); // Ouvre un fichier en mode "écriture"

    for (i = 0; i < NB_MAX_PLACE; i++) {
        if (!dossier[i].disponible) {
            // Insert la numéro de dossier dans le fichier
            fputs(dossier[i].num_dossier, fichier_w);
            fputc('\n', fichier_w);

            // Insert le nom du client dans le fichier
            fputs(dossier[i].nom, fichier_w);
            fputc('\n', fichier_w);

            // Insert le prénom du client dans le fichier
            fputs(dossier[i].prenom, fichier_w);
            fputc('\n', fichier_w);
        }
    }

    fclose(fichier_w); // Ferme le fichier
}
