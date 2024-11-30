//BELABBAS Melissa 21312498
//VOLAHASINA Nirina Jessica 21303391

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "ecosys.h"  

#define NB_PROIES 20
#define NB_PREDATEURS 20
#define MAX_ENERGIE 10.0



int main(void) {
    Animal *liste_proie = NULL;
    Animal *liste_predateur = NULL;
    Animal *nouvelle_liste_proie = NULL;
    Animal *nouvelle_liste_predateur = NULL;

    srand(time(NULL));

    // Création de 20 proies et 20 prédateurs
    for (int i = 0; i < NB_PROIES; i++) {
        int x = rand() % SIZE_X;
        int y = rand() % SIZE_Y;
        float energie = ((float)rand() / RAND_MAX) * MAX_ENERGIE;
        ajouter_animal(x, y, energie, &liste_proie);
    }

    for (int i = 0; i < NB_PREDATEURS; i++) {
        int x = rand() % SIZE_X;
        int y = rand() % SIZE_Y;
        float energie = ((float)rand() / RAND_MAX) * MAX_ENERGIE;
        ajouter_animal(x, y, energie, &liste_predateur);
    }

    // Écriture des listes dans un fichier
    FILE *f = fopen("ecosysteme.txt", "w");
    if (!f) {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    ecrire_ecosysteme(f, liste_proie);
    ecrire_ecosysteme(f, liste_predateur);
    fclose(f);

    // Lecture des listes dans de nouvelles variables
    f = fopen("ecosysteme.txt", "r");
    if (!f) {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    nouvelle_liste_proie = lire_ecosysteme(f);
    nouvelle_liste_predateur = lire_ecosysteme(f);
    fclose(f);

    // Vérification que les écosystèmes sont identiques
    if (comparer_ecosystemes(liste_proie, nouvelle_liste_proie) &&
        comparer_ecosystemes(liste_predateur, nouvelle_liste_predateur)) {
        printf("Les écosystèmes sont identiques\n");
    } else {
        printf("Les écosystèmes sont différents\n");
    }

    // Libérer la mémoire des listes
    liberer_liste_animaux(liste_proie);
    liberer_liste_animaux(liste_predateur);
    liberer_liste_animaux(nouvelle_liste_proie);
    liberer_liste_animaux(nouvelle_liste_predateur);

    return 0;
}

