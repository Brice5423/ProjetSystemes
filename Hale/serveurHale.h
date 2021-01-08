//
// Created by brice on 08/01/2021.
//

#ifndef PROJETSYSTEMES_SERVEURHALE_H
#define PROJETSYSTEMES_SERVEURHALE_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <netinet/in.h>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <signal.h>
# include <stdbool.h>
# include <time.h>
# include <pthread.h>

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
