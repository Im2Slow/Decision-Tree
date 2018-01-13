#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "noeud.h"
#include "donnees.h"

void Init(noeud * racine);
void construire_Arbre(noeud * racine,int hauteur,double minAccuracy,double maxAccuracy,int compteur,double minEchantillon);
double pourcentToRatio(double valeur);
void menu(noeud const * racine);
int getHauteur(noeud const * racine);
int max(int a, int b);
int getLargeur(noeud const * racine);
void Arborescence(noeud const * racine);
void Feuilles(noeud const * racine);
void print_feuille(noeud const * racine,char chemin[]);
void Prediction(noeud const * racine);
void Predict(noeud const * racine,double * newUser, double * precisionVar);

#endif