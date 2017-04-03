/*
 *Regroupe l'enssemble du pretraitement en UN seul appel(cf pretraitement_*)
 *
 *
 *L'utilisation de structure en particuluier d'une structure permettant de rendre les resultat en un seul parametre aurait ete interessant si ce n'etait la sifficulte considerable a les rendre totalement dynamique.
 *
 *@author:DUFRAIGNE ZANON 
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pretraitement_un_mot.h"
#include "pretraitement_conversion.h"
#include "constante.h"

int numero_mot_cle(char*,char**,int);
extern void pretraitement(const int nbre_mot_dico,const char **mot_du_dico,const char *reponse_theorique,const char *reponse_donnee,
			  /*
			   *UTILISES POU RENDRE LES RESULTATS
			   */
			  int ***adresse_offset_mot_cle_retour,
			  double ***adresse_fitness_retour,
			  int ***adresse_rang_retour,
			  int *nombre_mot_cle_retour,
			  int *nombre_mot_donne_retour,
			  int **poids_retour,   
			  char ***adresse_mot_cle_retour/*pour l'affichage de la reponse et la liberation de l'espace*/
			  ){
  


   /*TABLEAU DE TABLEAU DE FITNESS ET DE RANG*/
			  /*implementes sous forme de pointeur de pointeur de double et d'entier*/
			  /*
			   *L'espace pour les double et les entier sera reserve lors des methodes qui les remplissent
			   *L'espace necessaire pour acceuillir les pointeur de pointeur sera reserve juste apres l'initialisation
			   *
			   *Sur cette dimension les tableau on la taille de nombre_mot_donne
			   */

  char ***adresse_adresse_mot_cle;
  int **adresse_offset_mot_cle;
  double **adresse_fitness;
  int **adresse_rang;
  int i,j;
  char **mot_du_dico_svg=(char**)calloc(nbre_mot_dico,sizeof(char*));
  memcpy(mot_du_dico_svg,mot_du_dico,nbre_mot_dico*sizeof(char*));/*mot_du_dico_svg sera a chaque fois remis tel quel*/
  char **mot_du_dico_svg_pour_liberation=mot_du_dico_svg;/*car sera modifier et pb pour le free*/
  
  /* le fait que les mot soit deplacer n'est pas genant en soit mais peut ralentir un recherche car la liste n'est plus trie par ordre alphabetique
   *De plus permet de garder la reference AU PREMIER MOT et donc AU DEBUT DE LA ZONNE pour la liberation*/
  
  
  
  
  int nombre_mot_cle;
  int nombre_mot_donne;
  char **adresse_mot_cle;  /*Pointeur vers les mots cles(gardee pour que l'on puisse liberer l'espace)*/
  int *poids;              /*Vers les poids*/
  char **adresse_mot_donne;/*Vers les addresse de mots donnes dans la reponse de l'utilisateur*/
  
  /****
   ***/
  pretraitement_conversion(reponse_theorique,reponse_donnee,&adresse_mot_cle,&poids,&adresse_mot_donne,&nombre_mot_cle,&nombre_mot_donne);
  /**
   *nombre_mot_cle,nombre_mot_donneadresse_mot_cle,poids,adresse_mot_donne
   *Sont bien initialises
   **/
  adresse_fitness=(double**)calloc(nombre_mot_donne,sizeof(double*));
  adresse_rang=(int**)calloc(nombre_mot_donne,sizeof(int*));
  adresse_adresse_mot_cle=(char***)calloc(nombre_mot_donne,sizeof(char**));
  for(i=0;i<nombre_mot_donne;i++){
    *(adresse_adresse_mot_cle+i)=calloc(nombre_mot_cle,sizeof(char*));
    memcpy(*(adresse_adresse_mot_cle+i),adresse_mot_cle,nombre_mot_cle*sizeof(char*));
    
  }

  
  /*maintenant On doit remplir tout ces tableau pour tous les mots donnes
   */
  for(i=0;i<nombre_mot_donne;i++){
    if(DEBUG)
      printf("\nRANG\n");
    /*
     *Au retour (adressefitness+i),(adresserang+i) pointerons vers une zone memoire bien initialisee et reservee,de taille nbre_mot_cle double et nbre_mot_cle int
     */
    pretraitement_un_mot(*(adresse_mot_donne+i),nbre_mot_dico,mot_du_dico_svg,nombre_mot_cle,*(adresse_adresse_mot_cle+i),(adresse_fitness+i),(adresse_rang+i));
    
    memcpy(mot_du_dico_svg,mot_du_dico,nbre_mot_dico*sizeof(char*));
    if(DEBUG)
      printf("OK\n");
    
  }
  
    
  pretraitement_conversion_liberation_d_espace_mots(nombre_mot_donne,adresse_mot_donne);
  free(mot_du_dico_svg_pour_liberation);
  adresse_offset_mot_cle=(int**)calloc(nombre_mot_donne,sizeof(int*));
  for(i=0;i<nombre_mot_donne;i++){
    *(adresse_offset_mot_cle+i)=(int*)calloc(nombre_mot_cle,sizeof(int));
    for(j=0;j<nombre_mot_cle;j++){
      *(*(adresse_offset_mot_cle+i)+j)=numero_mot_cle(*(*(adresse_adresse_mot_cle+i)+j),adresse_mot_cle,nombre_mot_cle);
    }
    free(*(adresse_adresse_mot_cle+i));
  }
  free(adresse_adresse_mot_cle);
      
  *adresse_offset_mot_cle_retour=adresse_offset_mot_cle;
  *adresse_fitness_retour=adresse_fitness;
  *adresse_rang_retour=adresse_rang;
  *nombre_mot_cle_retour=nombre_mot_cle;
  *nombre_mot_donne_retour=nombre_mot_donne;
  *poids_retour=poids;
  *adresse_mot_cle_retour=adresse_mot_cle;/*pour l'affichage de la reponse et la liberation de l'espace libre*/
}


extern void pretraitement_liberation_de_l_espace(int **adresse_offset_mot_cle,char **adresse_mot_cle,int nombre_mot_cle,int *poids,int **adresse_rang,double **adresse_fitness,int nombre_mot_donne){
  int i;
  pretraitement_conversion_liberation_d_espace_mots(nombre_mot_cle,adresse_mot_cle);
  pretraitement_conversion_liberation_d_espace_poids(poids);

  /*
   *Libere l'espace pris par les mots cle et les poids ainsi que les diferents pointeurs qui y sont lies
   */
  for(i=0;i<nombre_mot_donne;i++){
    free(*(adresse_offset_mot_cle+i));
    /*Liberation des fitness et rang*/
    pretraitement_un_mot_liberation_de_l_espace(*(adresse_rang+i),*(adresse_fitness+i));
    /**/
  }
  free(adresse_offset_mot_cle);
  /*Liberation des adresse de fitness et rang*/
  free(adresse_rang);
  free(adresse_fitness);
  /**/
}


int numero_mot_cle(char *mot_a_trouver,char** mot_cles,int nbmot_cle){
  int i;
  for(i=0;i<nbmot_cle;i++){
    if(strcmp(mot_a_trouver,*(mot_cles+i))==0){
      return i;
    }
  }
  printf("MOT NON TROUVER(traitementreponse.c)");
  exit(1);
  return 0;
}
