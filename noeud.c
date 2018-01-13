#include "noeud.h"

noeud *creer_racine(matrice_donnees * mat,int id_espece)
{
	noeud *element = (noeud*)malloc(sizeof(noeud));
	if(element != NULL)
	{
		element->matrice = mat;
		element->id_espece = id_espece;
		element->fils_gauche = NULL;
		element->fils_droite = NULL;
		element->tab_lignes = NULL;
		element->nb_lignes = mat->nb_lignes;
		element->racine = NULL;
		element->precision = 0;
		element->indexVariable = 0;
		element->test = 2;
		element->mediane = 0;
	}
	return element;
}
noeud *creer_noeud(noeud * parent)
{
	noeud *element = (noeud*)malloc(sizeof(noeud));
	if(element != NULL)
	{
		element->matrice = parent->matrice;
		element->id_espece = parent->id_espece;
		element->fils_gauche = NULL;
		element->fils_droite = NULL;
		element->tab_lignes = NULL;
		element->nb_lignes = 0;
		element->racine = parent;
		element->precision = 0;
		element->indexVariable = 0;
		element->test = 2;
		element->mediane = 0;
	}
	return element;
}
bool associer_fils_gauche(noeud *parent, noeud *enfant)
{
	bool associer = false;
	if(parent!=NULL && enfant!=NULL)
	{
		if(parent->fils_gauche==NULL)
		{
			parent->fils_gauche = enfant;
			associer = true;
		}
	}
	return associer;
}
bool associer_fils_droite(noeud *parent, noeud *enfant)
{
	bool associer = false;
	if(parent!=NULL && enfant!=NULL)
	{
		if(parent->fils_droite==NULL)
		{
			parent->fils_droite = enfant;
			associer = true;
		}
	}
	return associer;
}
noeud *supprimer_noeud(noeud * element)
{
	if(element != NULL)
	{
		free(element->tab_lignes);
		free(element);
	}
	return NULL;
}
int choixVariables(noeud * parent)
{
	//double medianeFinale = 0;
	double * accuracyRight = (double*)malloc((parent->matrice->nb_colonnes -1) * sizeof(double));
	double * accuracyLeft = (double*)malloc((parent->matrice->nb_colonnes -1) * sizeof(double));
	double * medianPere = (double*)malloc((parent->matrice->nb_colonnes -1) * sizeof(double));

	int * nbrElemLeftPerVariables = (int*)malloc((parent->matrice->nb_colonnes -1) * sizeof(int));
	int * nbrElemRightPerVariables = (int*)malloc((parent->matrice->nb_colonnes -1) * sizeof(int));

	int nbrElemLeft, nbrElemRight;

	for(int j = 1; j<parent->matrice->nb_colonnes;j++)
	{
		nbrElemLeft = 0;
		nbrElemRight = 0;
		
		int nbrTrueLeft = 0;
		int nbrTrueRight = 0;
		//printf( "médiane : %lf\n " , median(parent, j));
		double medianVal = median(parent,j);
		int indexMat = 0;
		for(int i = 0; i<parent->nb_lignes;i++)
		{ // vérifier s'il s'agit de l'étape de split intitial
			if (parent->nb_lignes == parent->matrice->nb_lignes)
			{
				indexMat = i;
			} 
			else 
			{
				// l'index trouvé dans tab_lignes est l'index dans la matrice de base
				indexMat = parent->tab_lignes[i]; 
			}
			if (parent->matrice->matrice[indexMat][j]<= medianVal)
			{
				nbrElemLeft++;
				if (parent->matrice->matrice[indexMat][0] == parent->id_espece)
				{	
					nbrTrueLeft++;		
				}
			}
			else 
			{
				nbrElemRight++;
				if (parent->matrice->matrice[indexMat][0] == parent->id_espece)
				{	
					nbrTrueRight++;				
				}			
			}
		}
		// j varie de 1 à 4 => les tableaux de 0 à 3
		accuracyLeft[j-1] = (double)nbrTrueLeft/nbrElemLeft;		
		accuracyRight[j-1] = (double)nbrTrueRight/nbrElemRight;
		nbrElemLeftPerVariables[j-1] = (int) nbrElemLeft;
		nbrElemRightPerVariables[j-1] = (int) nbrElemRight;
		medianPere[j-1] = medianVal;
	}
//	afficherColonne(accuracyLeft, parent->matrice->nb_colonnes-1);
//	afficherColonne(accuracyRight, parent->matrice->nb_colonnes-1);
//	afficherTableau(nbrElemLeftPerVariables, parent->matrice->nb_colonnes-1);	
//	afficherTableau(nbrElemRightPerVariables, parent->matrice->nb_colonnes-1);
	double maxLeft = 0;
	double maxRight = 0;
	int indexLeft = 0;
	int indexRight = 0;
	int indexFinal = 0;
	for(int i= 0; i<parent->matrice->nb_colonnes-1;i++)
	{
//		printf("\n");
//		printf("précision gauche : %lf\n",accuracyLeft[i]);
//		printf("précision droite : %lf\n",accuracyRight[i]);
//		printf("colonne : %d \n",i);
		
		if(accuracyLeft[i]>maxLeft)
		{
			maxLeft = accuracyLeft[i];
			indexLeft = i;//index de accuracy
		}
		if(accuracyRight[i]>maxRight)
		{
			maxRight = accuracyRight[i];
			indexRight = i;
		}
	}
	if(maxLeft>maxRight)
	{
		indexFinal = indexLeft;
	}
	else
	{
		indexFinal = indexRight;
	}
	//printf("index final : %d\n",indexFinal);
	// 
	//medianeFinale = medianPere[indexFinal] ;
	// indexFinal varie de 0 à 3 => colonnes 1 à 4 
	parent->mediane = medianPere[indexFinal];
	parent->indexVariable = indexFinal;
	noeud * fils_gauche = creer_noeud(parent);
	fils_gauche->nb_lignes = nbrElemLeftPerVariables[indexFinal];
	fils_gauche->precision = accuracyLeft[indexFinal];
	fils_gauche->test = 0; // correspond à inférieur ou égal (<=)
	//printf("precision fils gauche : %lf",fils_gauche->precision);
	
	associer_fils_gauche(parent,fils_gauche);
	noeud * fils_droite = creer_noeud(parent);
	fils_droite->nb_lignes = nbrElemRightPerVariables[indexFinal];
	fils_droite->precision = accuracyRight[indexFinal];
	//printf("precision fils droite : %lf",fils_droite->precision);
	fils_droite->test = 1; // correspond à supérieur strictement (>)
	associer_fils_droite(parent,fils_droite);
	free(accuracyLeft);
	free(accuracyRight);
	free(nbrElemLeftPerVariables);
	free(nbrElemRightPerVariables);
	free(medianPere);
	accuracyLeft = NULL;
	accuracyRight=NULL;
	nbrElemLeftPerVariables = NULL;
	nbrElemRightPerVariables = NULL;
	return indexFinal+1;//index de la colonne dans la matrice
	}

void split(noeud * parent,int index)
{
	parent->fils_gauche->matrice->nb_colonnes = parent->matrice->nb_colonnes;
	parent->fils_gauche->matrice = parent->matrice; // matrice de base
	parent->fils_gauche->racine = parent; //un fils se point vers son parent
	parent->fils_gauche->tab_lignes = (int*)malloc(parent->fils_gauche->nb_lignes * sizeof(int));
		
	InitTab(parent->fils_gauche->tab_lignes,parent->fils_gauche->nb_lignes);
	parent->fils_droite->matrice->nb_colonnes = parent->matrice->nb_colonnes;
	parent->fils_droite->matrice = parent->matrice;
	parent->fils_droite->racine = parent;
	parent->fils_droite->tab_lignes = (int*)malloc(parent->fils_droite->nb_lignes * sizeof(int));

	InitTab(parent->fils_droite->tab_lignes,parent->fils_droite->nb_lignes);
	int indexMat = 0;
	int k = 0;
	int l =0;
//	printf("\nindex : %d\n",parent->indexVariable);
//	printf("\nmediane : %lf\n",parent->mediane);
	double medianVal = parent->mediane;
	for(int i = 0; i<parent->nb_lignes;i++)
	{	
		// vérifier s'il s'agit de l'étape de split intitial
		if (parent->nb_lignes == parent->matrice->nb_lignes)
		{
			indexMat = i;
		} 
		else 
		{
			indexMat = parent->tab_lignes[i];
		}
		if(parent->matrice->matrice[indexMat][index] <= medianVal)
		{
			parent->fils_gauche->tab_lignes[k++]= indexMat;
			//printf("tab_lignes fils_gauche : %d\n",indexMat);
		}
		else
		{
			parent->fils_droite->tab_lignes[l++]= indexMat;
			//printf("tab_lignes fils_droite : %d\n",indexMat);
		}
	}
//	printf("\n");
//	afficherTableau(parent->fils_droite->tab_lignes,parent->fils_droite->nb_lignes);
//	printf("\n");
//	printf("\n");
//	afficherTableau(parent->fils_gauche->tab_lignes,parent->fils_gauche->nb_lignes);
//	printf("\n");
}
double median(noeud * parent, int colonne)
{
	double medianValue = 0;
	if(parent->nb_lignes <2)
	{
		printf(" \néchantillon trop petit, médiane non définie (à 0 par défaut) \n");
	}
	else
	{
		int indexMat = 0; // index contenu dans dans tab_lignes
		double * medianArray = (double*)malloc(parent->nb_lignes * sizeof(double));
		for(int i = 0; i<parent->nb_lignes;i++)
		{ // vérifier s'il s'agit de l'étape de split intitial
			if (parent->nb_lignes == parent->matrice->nb_lignes)
			{
				indexMat = i;
			} 
			else 
			{
				// l'index trouvé dans tab_lignes est l'index dans la matrice de base
				indexMat = parent->tab_lignes[i]; 
			}
			medianArray[i] = parent->matrice->matrice[indexMat][colonne];
		}
		if(allElementsAreEqual(medianArray,parent->nb_lignes))
		{
			printf("\nMédiane non définie (à 0 par défaut), tous les éléments du tableau sont égaux\n");
		}
		else
		{
			
			tri_bulle(medianArray,parent->nb_lignes);
			//afficherColonne(medianArray,parent->nb_lignes);
			if(parent->nb_lignes % 2 == 0)
			{
				int p = (parent->nb_lignes-1)/2;
				
				medianValue = (medianArray[p] + medianArray[p+1])/2;
			}
			else
			{
				int p = (parent->nb_lignes)/2;
				medianValue = medianArray[p];
			}
			if(medianValue==medianArray[parent->nb_lignes-1])
			{	//comme le tableau est déja trié, on peut directement comparer avec la derniere case qui contient deja le maximum
				int i = 0;
				while(medianArray[parent->nb_lignes-i-1] == medianValue)// si on entre dans cette condition, c'est que le max et la mediane sont égales
				{
					i++; // i compte l'écart entre le max et la valeur juste en dessous
				}
				// vu qu'on est sûr de la valeur maximum grace au tri, il suffit de récuperer la premiere valeur différente du max en remontant le tableau depuis la fin
				medianValue = medianArray[parent->nb_lignes-i-1];
			}
		}
		free(medianArray);
	}
	return medianValue;
}

void tri_bulle(double tab[],int taille)
{
	double temp = 0;
	int i, inversion;
	do
	{
		inversion=0;
		for(i=0;i<taille-1;i++)
		{
			if (tab[i]>tab[i+1])
			{
				temp = tab[i];
				tab[i] = tab[i+1];
				tab[i+1] = temp;
				inversion=1;
			}
		}
		taille--;
	}
	while(inversion);
}
bool allElementsAreEqual(double tab[],int taille)
{
	double element = tab[0];
	for(int i=1; i<taille;i++)
	{
		if(element!=tab[i])
		{
			return false;
		}
	}
	return true;
}
void afficherColonne(double tab[],int taille)
{
	for(int i = 0; i<taille;i++)
	{
		printf("%lf ",tab[i]);
	}
}
void afficherTableau(int tab[],int taille)
{
	for(int i = 0; i<taille;i++)
	{
		printf("%d ",tab[i]);
	}
}
void InitTab(int tab[],int taille)
{
	for(int i = 0;i<taille;i++)
	{
		tab[i] = 0;
	}
}

noeud * liberer_noeud(noeud * parent)
{
	if(parent !=NULL)
	{
		liberer_noeud(parent->fils_droite);
		liberer_noeud(parent->fils_gauche);
		supprimer_noeud(parent);
	}	
	return parent;
}
bool est_feuille(noeud const *element)
{
	bool feuille = false;
	if(element != NULL && element->fils_gauche==NULL && element->fils_droite == NULL)
	{
		feuille = true;
	}
	return feuille;
}
void offset(int n)
{
	for(int i = 0;i<n;i++)
	{
		printf("  ");
	}
}
void affichage_arborescence(noeud const *arbre,int decal)
{
	if(arbre != NULL)
	{
		
		offset(decal+1);
		if(arbre->racine == NULL)
		{
			printf(" | racine - nb lignes : %d \n",arbre->nb_lignes);
		}
		else
		{
			printf(" |-%lf %% -%d - X%d",ratioTopourcent(arbre->precision),arbre->nb_lignes,arbre->racine->indexVariable + 1);
		}
		if(arbre->test == 1)
		{
			printf(" > %lf\n",arbre->racine->mediane);
		}
		else if(arbre->test == 0)
		{
			printf(" <= %lf\n",arbre->racine->mediane);
		}
		if(arbre->fils_gauche != NULL || arbre->fils_droite != NULL)
		{
		
			if(arbre->fils_gauche==NULL)
			{
				offset(decal+2);
				printf(" |-x\n");
			}
			affichage_arborescence(arbre->fils_gauche,decal+1);
			if(arbre->fils_droite==NULL)
			{
				offset(decal+1);
				printf(" |-x\n");
			}
			affichage_arborescence(arbre->fils_droite,decal+1);
		}
	}
}
double ratioTopourcent(double valeur)
{
	double newValue = 0;
	newValue = valeur*100;
	return newValue;
}


