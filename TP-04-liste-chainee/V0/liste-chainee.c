#include "liste-chainee.h"
#include <stdlib.h>
#include <stdio.h>

#define TODO NULL;

// retourne vrai si l est vide et faux sinon
bool estVide(Liste l) {
	return l == NULL;
}

// créer une liste d'un seul élément contenant la valeur v
Liste creer(Element v){
	Liste list = malloc(sizeof(Cellule));
	if(list != NULL){
		list -> val = v;
		list -> suiv = NULL;
	}
	return list;
}

// ajoute l'élément v en tete de la liste l
Liste ajoutTete(Element v, Liste l) {
	Liste list = creer(v);
	list -> suiv = l;
	return list;
}


void afficheElement(Element e) {
	printf("%i ",e);
}

// affiche tous les éléments de la liste l
// Attention, cette fonction doit être indépendante du type des éléments de la liste
// utiliser une fonction annexe affiche_element
// Attention la liste peut être vide !
// version itérative
void afficheListe_i(Liste l) {
	Liste p = l;
	while(!estVide(p)){
		afficheElement(p ->val);
		p = p->suiv;
	}
}

// version recursive
void afficheListe_r(Liste l) {
	if(!estVide(l))
	{
		afficheElement(l-> val);
	}
	afficheListe_r(l -> suiv);
}

void detruireElement(Element e) {}
// Détruit tous les éléments de la liste l
// version itérative
void detruire_i(Liste l) {
	Liste list = l;
	while(list != NULL){
		Liste suivant;
		detruireElement(list->val);
		suivant = list -> suiv;
		free(list);
		list=suivant;
	}
	free(list);
}

// version récursive
void detruire_r(Liste l) {
	if(l != NULL){
		detruireElement(l->val);
		detruire_r(l->suiv);
		free(l);
	}
}

// retourne la liste dans laquelle l'élément v a été ajouté en fin
// version itérative
Liste ajoutFin_i(Element v, Liste l) {
	Liste list = l;
	if (estVide(l)){
		return creer(v);
	}
	while(list -> suiv!= NULL){;
		list=list->suiv;
	}
	list -> suiv = creer(v);
	return l;
}

// version recursive
Liste ajoutFin_r(Element v, Liste l) {
	if(estVide(l)){
		return creer(v);
	}
	else{
		l -> suiv = ajoutFin_r(v, l->suiv);
		return l;
	}
	//return liste
}

// compare deux elements
bool equalsElement(Element e1, Element e2){
	return e1 == e2;
}

// Retourne un pointeur sur l'élément de la liste l contenant la valeur v ou NULL
// version itérative
Liste cherche_i(Element v,Liste l) {
	Liste liste = l;
	while(!equalsElement(liste -> val , v) && liste!= NULL){
		liste = liste -> suiv;
	}
	return liste;
}

// version récursive
Liste cherche_r(Element v,Liste l) {
	if(l == NULL || equalsElement(l -> val , v) ){
	return l;
	}
	else{
		return cherche_r(v, l-> suiv);
	}
}

// Retourne la liste modifiée dans la laquelle le premier élément ayant la valeur v a été supprimé
// ne fait rien si aucun élément possède cette valeur
// version itérative
Liste retirePremier_i(Element v, Liste l) {
	Liste precedent = l;
	if(estVide(l) || (!equalsElement(l -> val, v) && l -> suiv == NULL)){
		return NULL;
	}
	Liste l2 = l -> suiv;
	while(!equalsElement(l2 -> val , v) && l2 != NULL){
		
	}
	precedent -> suiv = l2 -> suiv;
	free(l2);
	return l;
}


// version recursive
Liste retirePremier_r(Element v, Liste l) {
	if (estVide( l -> suiv )){
		if(equalsElement(l-> val , v)){
			free(l);
			return NULL;
		}
		else
		{
			return l;
		}
	}

	if(equalsElement((l -> suiv)-> val , v) && (l -> suiv)!= NULL)
	{
		Liste suivant = l -> suiv;
		l -> suiv = (l -> suiv) -> suiv;
		free(suivant);
		return l;
	}
	el
	else{
		l ->suiv = retirePremier_r(v, l -> suiv);
		return l;
	}
}


void afficheEnvers_r(Liste l) {
	if(!estVide(l)){
	afficheEnvers_r(l -> suiv) -> suiv = l;
	}
}
