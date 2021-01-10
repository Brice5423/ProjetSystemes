//
// Created by brice on 08/01/2021.
//

#include "serveurHale.h"


int ma_socket;


void clearScreen() {
    system("cls");      //Vide l'écran
    system("clear");    //Vide l'écran
}

//--------------------------------------------------------------------------------------------------------------

void sauvegarde(Dossier *dossier) {
    FILE *fichier;
    int i = 0;
    fichier = fopen("save.txt", "w"); //Ouvre un fichier en mode "écriture"

    while (i < NB_DOSSIER) {
        if (!dossier[i].disponible) {
            fputs(dossier[i].num_dossier, fichier); //"file put string" sert à insérer du texte dans le fichier
            fputc('\n', fichier); //"file put char" insère un new line
            fputs(dossier[i].nom, fichier);
            fputc('\n', fichier);
            fputs(dossier[i].prenom, fichier);
            fputc('\n', fichier);
        }
        i++;
    }
    fclose(fichier); //ferme le fichier
}


//--------------------------------------------------------------------------------------------------------------


void *fonc(void *arg) {
    char buffer[512];
    char num_dossier[11];

    Arg *param;
    int c = 0;
    int i = -1;
    param = (Arg *) arg;

    read(param->client_socket, buffer, sizeof(buffer));
    if (buffer[0] == 'r') { //Faire une réservation
        while (c < NB_DOSSIER) {
            if (param->ensemble_dossiers[c].disponible) {   //Si le dossier est disponible...
                param->ensemble_dossiers[c].disponible = 0; //...on le réserve
                i = c;
                c = NB_DOSSIER;
            }
            c++;
        }

        if (i == -1)
            write(param->client_socket, "Toutes les réservations sont prises !", 128);
        else {
            write(param->client_socket, "Une réservation est possible !", 128);
            read(param->client_socket, buffer, sizeof(buffer)); //Récupération du nom du client
            param->ensemble_dossiers[i].nom = strdup(buffer); //Affectation du nom du client au dossier
            read(param->client_socket, buffer, sizeof(buffer)); //Récupération du prénom du client
            param->ensemble_dossiers[i].prenom = strdup(buffer); //Affectation du prénom du client au dossier

            c = 0;
            while (c < 10) { //Génération aléatoire d'un numéro de dossier
                num_dossier[c] = '0' + (rand() % 10);
                c++;
            }
            num_dossier[10] = '\0';
            param->ensemble_dossiers[i].num_dossier = strdup(num_dossier); //Affectation du numéro de dossier
            write(param->client_socket, num_dossier, sizeof(num_dossier)); //Envoi du numéro de dossier
            printf("- Le dossier numéro : %s a été réservé par %s %s\n",
                   num_dossier, param->ensemble_dossiers[i].nom, param->ensemble_dossiers[i].prenom);

            sauvegarde(param->ensemble_dossiers);
        }
    } else { //Annulation d'une réservation
        read(param->client_socket, buffer, sizeof(buffer)); //Récupération du nom du client

        read(param->client_socket, num_dossier, sizeof(num_dossier)); //Récupération du numéro de dossier
        c = 0;
        i = 0;
        while (c < NB_DOSSIER) { //Parcours des dossiers
            if (!param->ensemble_dossiers[c].disponible) {
                if (!strcmp(buffer, param->ensemble_dossiers[c].nom) &&
                    !strcmp(num_dossier,
                            param->ensemble_dossiers[c].num_dossier)) { //On vérifie qu'il s'agit du bon dossier afin de le supprimer
                    param->ensemble_dossiers[c].disponible = 1;
                    param->ensemble_dossiers[c].nom = NULL;
                    param->ensemble_dossiers[c].prenom = NULL;
                    param->ensemble_dossiers[c].num_dossier = NULL;
                    c = NB_DOSSIER;
                    i = 1;
                    write(param->client_socket, "Réservation annulée avec succès !", 128);
                    printf("- Le dossier numéro : %s a été annulé\n", num_dossier);
                    sauvegarde(param->ensemble_dossiers);
                }
            }
            c++;
        }
        if (!i) //Si pas de réservation faite auparavant
            write(param->client_socket, "Votre réservation est introuvable !", 128);
    }
    shutdown(param->client_socket, 2);
    close(param->client_socket);
};


//--------------------------------------------------------------------------------------------------------------


int main() {
    clearScreen();

    char buffer[512];
    char c;
    Dossier *Dos;

    FILE *fichier;
    struct sockaddr_in my_adr;
    struct sockaddr_in client_address;
    int long_addr;
    int client_socket;
    int i, j;

    pthread_t td;

    system("echo 'Adresse IP du serveur : ' && hostname -I | cut -d' ' -f1"); //Affiche l'adresse IP du serveur
    printf("_______________________\n\n");

    srand(time(NULL));

    bzero(&my_adr, sizeof(my_adr));
    my_adr.sin_port = htons(30000);
    my_adr.sin_family = AF_INET;
    my_adr.sin_addr.s_addr = htonl(INADDR_ANY);

    //Création de la socket
    if ((ma_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("Création de la socket échouée !\n");
        exit(0);
    }

    bind(ma_socket, (struct sockaddr *) &my_adr, sizeof(my_adr)); //Bind le serveur sur la socket
    listen(ma_socket, 5);
    long_addr = sizeof(client_address);

    Dos = (Dossier *) malloc(sizeof(Dossier) * NB_DOSSIER);


    fichier = fopen("save.txt", "r");
    i = 0;

    while (c != EOF && (c = fgetc(fichier)) != EOF) { //On parcourt le fichier de sauvegarde
        if (i == 0) {
            printf("****************************************************\n");
            printf("*                                                  *\n");
            printf("*    ------------------------------------------    *\n");
            printf("*   |   Précédentes réservations effectuées:   |   *\n");
            printf("*    ------------------------------------------    *\n");
            printf("*                                                  *\n");
            printf("****************************************************\n");
        }

        Dos[i].disponible = 0;
        Dos[i].num_dossier = malloc(sizeof(char) * 11);
        Dos[i].nom = malloc(sizeof(char) * 255);
        Dos[i].prenom = malloc(sizeof(char) * 255);

        Dos[i].num_dossier[0] = c;
        j = 1;
        while ((c = fgetc(fichier)) != '\n') { //On récupère le numéro de dossier
            Dos[i].num_dossier[j] = c;
            j++;
        }
        Dos[i].num_dossier[j] = '\0';


        j = 0;
        while ((c = fgetc(fichier)) != '\n') { //On récupère le nom de la personne
            Dos[i].nom[j] = c;
            j++;
        }


        j = 0;
        while ((c = fgetc(fichier)) && c != '\n') { //On récupère le prenom de la personne
            Dos[i].prenom[j] = c;
            j++;
        }
        printf("\nDossier :\n\n");
        printf("%s\t-\t%s\t-\t%s\n\n", Dos[i].num_dossier, Dos[i].nom,
               Dos[i].prenom); //Affichage des clients déjà inscrits
        i++;
    }

    fclose(fichier);

    while (i < NB_DOSSIER) { //On initialise les fichiers restants
        Dos[i].disponible = 1;
        Dos[i].num_dossier = NULL;
        Dos[i].nom = NULL;
        Dos[i].prenom = NULL;

        i++;
    }
    printf("_______________________\n\n");
    printf("Actions ...\n\n"); // A partir d'ici, les actions des clients seront affichés en dessous de ce texte

    while ((client_socket = accept(ma_socket, (struct sockaddr *) &client_address, &long_addr)) >
           0) { //On attend une ou des connexion(s)
        Arg *T;

        T = (Arg *) malloc(sizeof(Arg) * 1);
        T->client_socket = client_socket;
        T->ensemble_dossiers = Dos;

        pthread_create(&td, NULL, fonc, T);

    }

    close(ma_socket);
}
