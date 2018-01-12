#ifndef NOEUD_H
#define NOEUD_H
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "donnees.h"

typedef struct _noeud
{
	//accès noeuds racine/fils
	struct _noeud * racine;
	struct _noeud * fils_droite;
	struct _noeud * fils_gauche;
	//accès aux données
	matrice_donnees * matrice;
	//valeur de la variable à déterminer
	int id_espece;
	//critères de division
	int indexVariable;
	double mediane;
	double precision;
	int test;
	// liste individus
	int * tab_lignes;
	int nb_lignes;
}noeud;

noeud * creer_racine(matrice_donnees * mat,int id_espece);
noeud * creer_noeud(noeud * parent);
noeud * supprimer_noeud(noeud * element);
noeud * liberer_noeud(noeud * parent);

bool associer_fils_gauche(noeud *parent, noeud *enfant);
bool associer_fils_droite(noeud *parent, noeud *enfant);
int choixVariables(noeud * racine);
void split(noeud * parent, int index);
double median(noeud *parent, int colonne);
bool est_feuille(noeud const *element);
void offset(int n);
void affichage_arborescence(noeud const *arbre,int decal);
// Usage var = mediane ( matrice contenant les données, colonne de la matrice sur laquelle on souhaite obtenir la médiane);

void tri_bulle(double tab[],int taille);

bool allElementsAreEqual(double tab[], int taille);

void afficherColonne(double tab[],int taille);
void afficherTableau(int tab[], int taille);
void InitTab(int tab[],int taille);

#endif