#include "linkedList.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void afficheElement(Element e){
    printf((char *) e);
}

void detruireElement(Element e) {
    free((char *) e);
}
bool equalsElement(Element a, Element v){
    return (strcmp((char*)a,(char*)v)==NULL);
}

int main(void){

    char* e = "rrrza";
    Liste liste;
    char* f = "ejngaie";
    liste= NULL;

    liste = ajoutTete(e,liste);
    liste = ajoutTete(f,liste);
    liste = ajoutTete(e,liste);

    afficheListe_i(liste);
    detruire_r(liste);

    return EXIT_SUCCESS;
}