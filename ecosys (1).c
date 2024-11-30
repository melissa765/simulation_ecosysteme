//BELABBAS Melissa 21312498
//VOLAHASINA Nirina Jessica 21303391



#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "ecosys.h"


float p_ch_dir = 0.01;
int temps_repousse_herbe = -15;
float p_reproduce_proie = 0.4;
float p_reproduce_predateur = 0.5;

/* PARTIE 1*/
/* Fourni: Part 1, exercice 4, question 2 */
Animal *creer_animal(int x, int y, float energie) {
  Animal *na = (Animal *)malloc(sizeof(Animal));
  assert(na);
  na->x = x;
  na->y = y;
  na->energie = energie;
  na->dir[0] = rand() % 3 - 1;
  na->dir[1] = rand() % 3 - 1;
  na->suivant = NULL;
  return na;
}


/* Fourni: Part 1, exercice 4, question 3 */
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal);
  assert(!animal->suivant);
  animal->suivant = liste;
  return animal;
}

/* A faire. Part 1, exercice 6, question 2 */
// Fonction pour ajouter un animal à une position spécifique dans la liste
void ajouter_animal(int x, int y, float energie, Animal **liste_animal) {
    assert(x >= 0 && x < SIZE_X);
    assert(y >= 0 && y < SIZE_Y);
    Animal *nouvel_animal = creer_animal(x, y, energie);
    nouvel_animal->suivant = *liste_animal;
    *liste_animal = nouvel_animal;
}


/* A Faire. Part 1, exercice 5, question 5 */
void enlever_animal(Animal **liste, Animal *animal) {
    Animal *courant = *liste;
    Animal *precedent = NULL;

    // Parcourir la liste pour trouver l'animal à supprimer
    while (courant != NULL) {
        if (courant == animal) {
            // Si l'animal à supprimer est le premier de la liste
            if (precedent == NULL) {
                *liste = courant->suivant;
            } else {
                precedent->suivant = courant->suivant;
            }
            free(courant);  // Libérer la mémoire de l'animal supprimé
            return;
        }
        precedent = courant;
        courant = courant->suivant;
    }
}


/* A Faire. Part 1, exercice 6, question 7 */

// Fonction pour libérer la mémoire allouée pour une liste d'animaux
Animal* liberer_liste_animaux(Animal *liste) {
    Animal* tmp=liste;
    while(tmp!=NULL){
    	liste=tmp->suivant;
    	free(tmp);
    	tmp=liste;
    }
    return NULL;
}



/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_rec(Animal *la) {
  if (!la) return 0;
  return 1 + compte_animal_rec(la->suivant);
}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_it(Animal *la) {
  int cpt=0;
  while (la) {
    ++cpt;
    la=la->suivant;
  }
  return cpt;
}



/* Part 1. Exercice 5, question 1, ATTENTION, ce code est susceptible de contenir des erreurs... */


void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
    unsigned int i, j;
    char **ecosys = malloc(SIZE_Y * sizeof(char *));
    for (i = 0; i < SIZE_Y; ++i) {
        ecosys[i] = malloc(SIZE_X * sizeof(char));
    }

    Animal *pa = NULL;

    /* on initialise le tableau */
    for (i = 0; i < SIZE_Y; ++i) {
        for (j = 0; j < SIZE_X; ++j) {
            ecosys[i][j] = ' ';
        }
    }

    /* on ajoute les proies */
    pa = liste_proie;
    while (pa) {
        assert(pa->x >= 0 && pa->x < SIZE_X);
        assert(pa->y >= 0 && pa->y < SIZE_Y);
        ecosys[pa->y][pa->x] = '*';  // Utiliser `ecosys[y][x]` pour la bonne orientation
        pa = pa->suivant;
    }

    /* Ajouter les prédateurs */
    pa = liste_predateur;
    while (pa) {
        if ((ecosys[pa->y][pa->x] == '@') || (ecosys[pa->y][pa->x] == '*')) {//proie aussi present
            ecosys[pa->y][pa->x] = '@';
        } else {
            ecosys[pa->y][pa->x] = 'O';
        }
        pa = pa->suivant;
    }

    /* on affiche le tableau  */
    printf("+");
    for (j = 0; j < SIZE_X; ++j) {
        printf("-");
    }
    printf("+\n");
    for (i = 0; i < SIZE_Y; ++i) {
        printf("|");
        for (j = 0; j < SIZE_X; ++j) {
            putchar(ecosys[i][j]);
        }
        printf("|\n");
    }
    printf("+");
    for (j = 0; j < SIZE_X; ++j) {
        printf("-");
    }
    printf("+\n");

    /* Afficher le nombre de proies et de prédateurs */
    int nbproie = compte_animal_it(liste_proie);
    int nbpred = compte_animal_it(liste_predateur);

    printf("Nb proies : %5d\tNb prédateurs : %5d\n", nbproie, nbpred);

    /* Libérer la mémoire allouée pour ecosys */
    for (i = 0; i < SIZE_Y; ++i) {
        free(ecosys[i]);
    }
    free(ecosys);
}


void clear_screen() {
  printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
}

/* PARTIE 2*/

void bouger_animaux(Animal *la) {
    Animal* ap=la;
    while(ap){
    	if(rand()/(float)RAND_MAX<p_ch_dir){
    		ap->dir[0]=rand()%3-1;
    		ap->dir[1]=rand()%3-1;
    	}
    	ap->x= (ap->x + ap->dir[0] + SIZE_X) % SIZE_X;
    	ap->y= (ap->y + ap->dir[1] + SIZE_Y) % SIZE_Y;
    	ap=ap->suivant;
    }
}

void reproduce(Animal **liste_animal, float p_reproduce) {
    Animal *current = liste_animal ? * liste_animal : NULL;

    while (current) {
        if (((float)rand() / RAND_MAX) < p_reproduce) {
            ajouter_animal(current->x ,current->y , current->energie/2, liste_animal);
            current->energie=current->energie/2;
        }
        current = current->suivant;
    }
    
}


// Fonction pour écrire l'écosystème dans un fichier
void ecrire_ecosys(FILE *f, Animal *liste) {
    int count = compte_animal_it(liste);
    fprintf(f, "%d\n", count);  // Écrire le nombre d'animaux

    Animal *courant = liste;
    while (courant) {
        fprintf(f, "%d %d %f\n", courant->x, courant->y, courant->energie);
        courant = courant->suivant;
    }
}

// Fonction pour lire un nombre spécifique d'animaux depuis le fichier
Animal *lire_ecosys(FILE *f) {
    Animal *liste = NULL;
    int count;
    fscanf(f, "%d", &count);  // Lire le nombre d'animaux

    int x, y;
    float energie;
    for (int i = 0; i < count; i++) {
        if (fscanf(f, "%d %d %f", &x, &y, &energie) == 3) {
            ajouter_animal(x, y, energie, &liste);
        }
    }
    return liste;
}





void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) {
    bouger_animaux(*liste_proie);
    Animal *current = *liste_proie;
    Animal *prev = NULL;
    while (current != NULL) {
        current->energie -= 1;

        if (current->energie <0) {
            Animal *to_delete = current;
            if (prev == NULL) {
                *liste_proie = current->suivant; 
            } else {
                prev->suivant = current->suivant;
            }
            current = current->suivant;
            free(to_delete);
        } else {
            prev = current;
            current = current->suivant;
        }
    }

    reproduce(liste_proie, p_reproduce_proie);

}




Animal *animal_en_XY(Animal *l, int x, int y) {
    while (l != NULL) {
        if (l->x == x && l->y == y) {
            return l;
        }
        l = l->suivant;
    }
    return NULL;
}


void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie) {
	 bouger_animaux(*liste_predateur);
    Animal *current = *liste_predateur;
    Animal *prev = NULL;

    while (current != NULL) {
        current->energie -= 1;  

        // Vérifie si le prédateur mange une proie à sa position
        Animal *proie = animal_en_XY(*liste_proie, current->x, current->y);
        if (proie != NULL) {
            current->energie += proie->energie;  // Le prédateur gagne l'énergie de la proie
            enlever_animal(liste_proie, proie);  // Supprime la proie de la liste
        }

        // Si le prédateur n'a plus d'énergie, il meurt
        if (current->energie < 0) {
            Animal *to_delete = current;
            if (prev == NULL) {
                *liste_predateur = current->suivant;
            } else {
                prev->suivant = current->suivant;
            }
            current = current->suivant;
            free(to_delete);
        } else {
            prev = current;
            current = current->suivant;
        }
    }

    // Reproduction des prédateurs
    reproduce(liste_predateur, p_reproduce_predateur);
}


/* Part 2. Exercice 5, question 2 */
void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){
	/*A Completer*/
	for (int i=0; i<SIZE_X; i++){
		for (int j=0; j<SIZE_Y; j++){
		monde[i][j]+=1;
		}
	}

}

