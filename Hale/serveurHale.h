//
// Created by brice on 08/01/2021.
//

#ifndef PROJETSYSTEMES_SERVEURHALE_H
#define PROJETSYSTEMES_SERVEURHALE_H

// se trouve dans le sinnn //
# include <sys/socket.h>
# include <netinet/in.h>

// on les a par defaut //
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>
# include <time.h>

// On les a pas //
# include <signal.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <sys/types.h>


# define NB_DOSSIER 100 //Nombre de réservations max.


struct dossier { // crée une structure dossier
    int disponible;
    char *num_dossier;
    char *nom;
    char *prenom;
};
typedef struct dossier Dossier; // crée un type de type strycture dossier

struct arg { // crée une structure arg
    int client_socket;
    Dossier *ensemble_dossiers;
};
typedef struct arg Arg; // crée un type de type strycture arg

#endif //PROJETSYSTEMES_SERVEURHALE_H
