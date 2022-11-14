/*

toujours des segmentation fault, valgrind --leak-check=yes ./a.out renvoie l'erreur :
==564397== Invalid write of size 8
==564397==    at 0x109258: main (in /home/kb/Desktop/mlod/mlod-algo/TP-04-liste-chainee/V2/a.out)
==564397==  Address 0x0 is not stack'd, malloc'd or (recently) free'd.
Cela semble provenir des lignes 39 à 45, mais je n'arrive pas à corriger mon problème...

*/


#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"
#include "linkedListOfMusic.h"
#include <string.h>
#include <errno.h>

    


int main(void){


    Liste listeMusiques = NULL;
    FILE* f;
    char ligne[256];
    char* ligneActuelle;

    f = fopen("music.csv","r");
    ligneActuelle = fgets(ligne,256,f);

    while (ligneActuelle != NULL)
    {
        Music* derniereMusique = NULL;
        
        ligneActuelle = strdup(ligne);

        derniereMusique -> name = strsep(&ligneActuelle,",");
        derniereMusique -> artist = strsep(&ligneActuelle,",");
        derniereMusique -> album = strsep(&ligneActuelle,",");
        derniereMusique -> genre = strsep(&ligneActuelle,",");
        derniereMusique -> discNumber = atoi(strsep(&ligneActuelle,","));
        derniereMusique -> trackNumber = atoi(strsep(&ligneActuelle,","));
        derniereMusique -> year = atoi(strsep(&ligneActuelle,",") );

        listeMusiques = ajoutFin_r(derniereMusique, listeMusiques);
        
        ligneActuelle = fgets(ligne,256,f);
    }

    fclose(f);

    afficheListe_i(listeMusiques);

    listeMusiques = trierListe_r(listeMusiques);
    afficheListe_i(listeMusiques);

    detruire_i(listeMusiques);

    return EXIT_SUCCESS;
    }