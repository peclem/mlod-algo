#include "linkedListOfMusic.h"
#include "linkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void afficheElement(Element e) {
    printf("(%s) , (%s) , (%i)", ((Music*)e)->name, ((Music*)e)->artist, ((Music*)e)->year);
}

void detruireElement(Element e)
 {
    free((( Music* ) e ) -> album);
    free((( Music* ) e ) -> artist);
    free((( Music* ) e ) -> name);
    free((( Music* ) e ) -> genre);
    free((Music*) e );
}

bool equalsElement(Element e1, Element e2) {

    Music* E1 = ( Music* )e1;
    Music* E2 = ( Music* )e2;
    return(strcmp(E1 -> album, E2 -> album) && strcmp(E1 -> name, E2 -> name) && strcmp(E1 -> artist, E2 -> artist) );
}


bool estPlusRecent(Element e1, Element e2){
    Music* E1 = ( Music* )e1;
    Music* E2 = ( Music* )e2;
    return(E1 -> year > E2 -> year);
}

Liste trierListe_r(Liste liste)
{
    Liste triee = NULL;
    return trierListe_aux(triee, liste);
}

Liste trierListe_aux(Liste triee, Liste liste)
{
    Liste aTrier = liste;
    if (estVide(liste))
    {
        return triee;
    }
    Liste p = triee;
    while(estPlusRecent(p, aTrier) && (estVide(p -> suiv) || !estPlusRecent(p -> suiv, aTrier)))
    {
        Liste psuiv = p -> suiv;
        p -> suiv = aTrier;
        aTrier = aTrier -> suiv;
        liste -> suiv = psuiv;
        p = psuiv;
    }
    return trierListe_aux(triee, aTrier);
}