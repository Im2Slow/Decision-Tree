#include "main.h"
void construire_Arbre(noeud * racine, int hauteurMax,double minAccuracy,double maxAccuracy,int compteur, double minEchantillon)
{
//	printf("nb_lignes : %d\n",racine->nb_lignes);
//	printf("compteur : %d\n",compteur);
	if(compteur == 0)
	{
		racine->precision = minAccuracy;//Init par défaut
	}
	if(compteur <= hauteurMax && racine->precision >= minAccuracy && racine->precision <= maxAccuracy && racine->nb_lignes >= (racine->matrice->nb_lignes)*minEchantillon)
	{
		split(racine,choixVariables(racine));
		construire_Arbre(racine->fils_droite,hauteurMax,minAccuracy,maxAccuracy,compteur+1,minEchantillon);
		construire_Arbre(racine->fils_gauche,hauteurMax,minAccuracy,maxAccuracy,compteur+1,minEchantillon);
	}	
}
double pourcentage(double valeur)
{
	double newValue = 0;
	newValue = valeur/100;
	return newValue;
}
void menu(noeud const * racine)
{
	int hauteur = 0;
	int largeur = 0;
	bool stop = false;
	while(stop != true)
	{
		printf("\nMENU \n Veuillez saisir le chiffre correspondant à la fonctionnalité que vous souhaitez executer \n 1) Afficher la hauteur de l'arbre \n 2) Afficher la largeur de l'arbre (le nombre de feuille) \n 3) Afficher l'arbre sous forme arborescente \n 4) Afficher les feuilles \n 5) Prédire \n 6) Quitter le programme \n");
		int input;
		scanf("%d%*c",&input);
		switch(input)
		{
			case 1:
				hauteur = exo1(racine);
				printf("\n hauteur de l'arbre : %d \n Appuyez sur ENTREE pour continuer\n",hauteur);
				scanf("%*c");
				break;
				
			case 2:
				largeur = exo2(racine);
				printf("\n largeur de l'arbre : %d \n Appuyez sur ENTREE pour continuer\n",largeur);
				scanf("%*c");
				break;
			case 3:
				exo3(racine);
				break;
				
			case 4:
				exo4(racine);
				break;
				
			case 5:
				exo5(racine);
				break;
				
			case 6:
				stop = true;
				break;
			default:
				break;
		}
	}
}
int exo1(noeud const * racine)
{
	if(racine == NULL)
	{
		return 0;
	}
	else
	{
		return 1 + max(exo1(racine->fils_gauche),exo1(racine->fils_droite));
	}
}
int max(int a, int b)
{
	if(a>b)
	{
		return a;
	}
	else
	{
		return b;
	}
}
int exo2(noeud const * racine)//init 0
{
	if(racine == NULL)
	{
		return 0;
	}
	else if(est_feuille(racine))
	{
		return 1;
	}
	else
	{
		return exo2(racine->fils_droite) + exo2(racine->fils_gauche);
	}
}
void exo3(noeud const * racine)
{
	printf("\nordre affichage : |-precision - nombre d'individus - variable choisie - critère(<= ou >) - médiane pour la variable choisie \n\n");
	affichage_arborescence(racine,0);
	scanf("%*c");
}
void exo4(noeud const * racine)
{
	//printf("Les feuilles sont ordonnées de manière décroissante : les dernières feuilles affichées sont les plus proches de la racine\nsi deux feuilles sont issues du même parent, elles ont également le même numero\n");
	char * str = (char*)malloc(sizeof(char));
	str[0] = '\0';
	print_feuille(racine,str);
	free(str);
	scanf("%*c");
}
void exo5(noeud const * racine)
{
	double * newElement = (double*)malloc((racine->matrice->nb_colonnes-1) * sizeof(double));
	double precisionVar = 0;
	printf("\nVeuillez entrer les valeurs des 4 variables Xi d'un nouvel individu\n");
	for(int i = 0; i<racine->matrice->nb_colonnes-1;i++)
	{
		printf("\nValeur de X%d ?\n",i+1);
		scanf("%lf",&newElement[i]);
	}
	Predict(racine,newElement, &precisionVar);
	printf("\nPrécision pour l'espèce %d : %lf\n",racine->id_espece,precisionVar);
	free(newElement);
	scanf("%*c");
}
void Predict(noeud const * racine,double * newUser, double * precisionVar)
{
	// indexVariable varie de 0 à 3 : X1 à X4
//	printf("\nindexVariable : %d\n",racine->indexVariable+1);
//	printf("\n tab[indexVariable] : %lf\n",newUser[racine->indexVariable]);
//	printf("\nmediane : %lf\n",racine->mediane);
//	printf("\nprecision : %lf\n",racine->precision);
	
	if(newUser[racine->indexVariable] <= racine->mediane)
	{
		if(racine->fils_gauche != NULL)
		{
			Predict(racine->fils_gauche,newUser, precisionVar);
		}
		else
		{
			*precisionVar= racine->precision;
		}
	}
	else
	{
		if(racine->fils_droite != NULL)
		{
			Predict(racine->fils_droite,newUser, precisionVar);
		}
		else
		{
			*precisionVar= racine->precision;
		}
	}
}
void print_feuille(noeud const * racine,char chemin[])
{
	char buf[256];
	buf[0] = '\0';
	if(racine->test==2)
	{
		sprintf(buf," r ");
	}
	if(racine->test==0)
	{
		sprintf(buf,"\\ X%d <= %lf",racine->racine->indexVariable + 1, racine->racine->mediane);
	}
	if(racine->test==1)
	{
		sprintf(buf,"\\ X%d > %lf",racine->racine->indexVariable + 1, racine->racine->mediane);
	}
	//char * toAdd = str_append(chemin,buf);
	char * toAdd = (char*)malloc((strlen(chemin) + strlen(buf))*sizeof(char));
	toAdd[0] = '\0';
	//printf("taille pre-append : %lu\n",strlen(toAdd));
	strcat(toAdd,chemin);
	//printf("\ntoAdd gets chemin : %s\n",toAdd);
	strcat(toAdd,buf);
	//printf("\ntoAdd gets buf : %s\n",toAdd);
	if(est_feuille(racine))
	{
		printf("\nFeuille trouvée -> precision : %lf , nombre individus : %d, chemin : %s\n",racine->precision,racine->nb_lignes,toAdd);
	}
	else
	{
		print_feuille(racine->fils_gauche,toAdd);
		print_feuille(racine->fils_droite,toAdd);
	}
	//printf("taille post-append : %lu\n",strlen(toAdd));
	free(toAdd);
}
void Init(noeud * racine)
{
	int hauteurMax = 0;
	double minAccuracy = 0;
	double maxAccuracy = 0;
	double minEchantillon = 0;
	printf("Choisissez le seuil minimal de précision en pourcentage. Exemple : écrivez 10 pour 10 pourcents\n");
	scanf(" %lf",&minAccuracy);
	minAccuracy = pourcentage(minAccuracy);

	printf("Choisissez le seuil maximal de précision en pourcentage. Exemple : écrivez 90 pour 90 pourcents\n");
	scanf(" %lf",&maxAccuracy);
	maxAccuracy = pourcentage(maxAccuracy);
	
	printf("Choisissez le pourcentage minimum d'individus par échantillons. Exemple : écrivez 10 pour 10 pourcents de la taille de l'échantillon de départ\n");
	scanf(" %lf",&minEchantillon);
	minEchantillon = pourcentage(minEchantillon);
	
	printf("Saisissez un entier correspondant à la hauteur maximum que l'arbre peut avoir :\n");
	scanf(" %d",&hauteurMax);
	
	construire_Arbre(racine,hauteurMax,minAccuracy,maxAccuracy,0,minEchantillon);
}
int main(int argc, char **argv)
{
	matrice_donnees * mat = charger_donnees("iris.txt");
	int id_espece = 0;
	
	printf("Choisissez l'espèce d'iris à prédire :\n1 pour iris setosa\n2 pour iris versicolor\n3 pour iris virginica\n");
	scanf(" %d",&id_espece);
	
	noeud * racine = creer_racine(mat,id_espece);
	Init(racine);
//	printf("precision root: %lf, ",racine->precision);
//	printf("precision droite : %lf, ",racine->fils_droite->precision);
//	printf("precision gauche : %lf \n",racine->fils_gauche->precision);
	menu(racine);
	mat = liberer_donnees(mat);
	racine= liberer_noeud(racine);
	return 0;
}