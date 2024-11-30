//BELABBAS Melissa 21312498
//VOLAHASINA Nirina Jessica 21303391


#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include "ecosys.h"

#define NB_PROIES 20
#define NB_PREDATEURS 20
#define T_WAIT 40000 
#define MAX_ITER 200


int main(void) {
    srand(time(NULL));

    Animal *liste_proie = NULL;
    Animal *liste_predateur = NULL;
    int monde[SIZE_X][SIZE_Y] = {0};

    // Initialisation des proies
    for (int i = 0; i < NB_PROIES; i++) {
        int x = rand() % SIZE_X;
        int y = rand() % SIZE_Y;
        float energie = 5.0 + ((float)rand() / RAND_MAX) * 5.0; // Énergie initiale de 5 à 10
        ajouter_animal(x, y, energie, &liste_proie);
    }

    // Initialisation des prédateurs
    for (int i = 0; i < NB_PREDATEURS; i++) {
        int x = rand() % SIZE_X;
        int y = rand() % SIZE_Y;
        float energie = 20.0; // Énergie initiale des prédateurs
        ajouter_animal(x, y, energie, &liste_predateur);
    }

    // Affichage initial
    printf("État initial de l'écosystème :\n");
    afficher_ecosys(liste_proie, liste_predateur);

    int iteration = 0;
    while ((compte_animal_it(liste_proie) > 0 || compte_animal_it(liste_predateur) > 0) && iteration < MAX_ITER) {
    rafraichir_proies(&liste_proie, monde);             // Mise à jour des proies
    rafraichir_predateurs(&liste_predateur, &liste_proie);  // Mise à jour des prédateurs
    afficher_ecosys(liste_proie, liste_predateur);
    usleep(T_WAIT);  // Pause pour observer chaque cycle
    iteration++;
}


    // Affichage final
    printf("Nombre final de proies : %d\n", compte_animal_it(liste_proie));
    printf("Nombre final de prédateurs : %d\n", compte_animal_it(liste_predateur));

    // Libération de la mémoire
    liberer_liste_animaux(liste_proie);
    liberer_liste_animaux(liste_predateur);

    return 0;
}

