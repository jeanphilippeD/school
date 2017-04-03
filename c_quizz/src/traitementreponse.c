/*
 *traitementreponse.c
 *Antecedent:Les donnes envoyees sont corectes
 *           nombre_mot_dico,mot_du_dico ont ete initialise
 *           reponse_theorique pointe sur la reponse theorique sorite du fichier
 *           reponse_donnee pointe sur la reponse utilisateur
 *
 *Consequent:rend le nombre de points que vaut la reponse en double
 *
 *@author:DUFRAIGNE ZANON 
 */


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pretraitement.h"
#include "constante.h"
          
extern double traitementreponse(const int nombre_mot_dico,const char **mot_du_dico,const char *reponse_theorique,const char *reponse_donnee){
  double nbre_point_total=0;
  
  int nombre_mot_cle=0;
  int nombre_mot_donne=0;
  int *poids=NULL;
  char **adresse_mot_cle=NULL;/*Contient l'adresse necessaire a la liberation de l'espace des mots cles*/
  int **adresse_offset_mot_cle=NULL;
  double **adresse_fitness=NULL;
  int **adresse_rang=NULL;
  
  int *rang_min_pour_un_mot_cle=NULL;
  double *fitnessde_rang_min_pour_un_mot_cle=NULL;
  /****
   ***/
  int n=0;
  int m=0;

  
  pretraitement(nombre_mot_dico,mot_du_dico,reponse_theorique,reponse_donnee,&adresse_offset_mot_cle,&adresse_fitness,&adresse_rang,&nombre_mot_cle,&nombre_mot_donne,&poids,&adresse_mot_cle);
  
  rang_min_pour_un_mot_cle=calloc(nombre_mot_cle,sizeof(int));
  fitnessde_rang_min_pour_un_mot_cle=calloc(nombre_mot_cle,sizeof(double));
  for(m=0;m<nombre_mot_cle;m++){
    *(rang_min_pour_un_mot_cle+m)=nombre_mot_dico;
    *(fitnessde_rang_min_pour_un_mot_cle)=-10000.0;/*le resultat est deja noye*/
  }
  /**
   **/

  /*
   *RAPPEL:
   * ces differentes variables sont initialisees par la methode precedente.
   *
   *adresse_adresse_mot_cle:a utiliser maintenant comme un tableau de tableau de pointeur vers les mots cles.
   *poids:a utiliser maintenant comme un tableau de poid.
   *adresse_mot_donne:a utiliser maintenant comme un tableau de pointeur vers les mots donnes
   *nombre_mot_th:represente le nbre de mot de la reponse theorique
   *nombre_mot_donne:represente le nbre de mot de la reponse donne
   *adresse_rang:tablau:1ere dim taille nombre_mot_donne 2eme dim nombre_mot_th
   *adresse_fitness:tablau:1ere dim taille nombre_mot_donne 2eme dim nombre_mot_th
   */


  /*
   *Les deux tableau de pointeur sont initialise.
   *On a un acces direct au mot de la reponse theorique et de la reponse donnee sur lesqueles on a effectuer un pretraitement
   */
  
  int tmp;/*indique le vrai rang du mot cle*/
  /*adresse_adresse_mot_cle et adresse_mot_cle on des pointeur vers les mot qui occupent une seule zone memoire tmp=m+(*(*(adresse_adresse_mot_cle+n)+m)-(adresse_mot_cle+m)=*(*(adresse_adresse_mot_cle+n)+m)+adresse_mot_cle*/

  /*
    for(n=0;n<nombre_mot_donne;n++){
    for(m=0;m<nombre_mot_cle;m++){
      tmp=*(*(adresse_offset_mot_cle+n)+m);
      
    }
    }*/
  
  for(n=0;n<nombre_mot_donne;n++){
    for(m=0;m<nombre_mot_cle;m++){
      tmp=*(*(adresse_offset_mot_cle+n)+m);
      if(DEBUG){
	printf("Pour le rang %d %d %d mt %d %s\n",n,m,*(*(adresse_rang+n)+m),tmp,*(adresse_mot_cle+tmp));
      }
      if(*(*(adresse_rang+n)+m)<*(rang_min_pour_un_mot_cle+tmp)){
	*(rang_min_pour_un_mot_cle+tmp)=*(*(adresse_rang+n)+m);
	*(fitnessde_rang_min_pour_un_mot_cle+tmp)=*(*(adresse_fitness+n)+m);
      }
    }
  }	
  
  for(m=0;m<nombre_mot_cle;m++){
    
    
    double d=((double)*(poids+m))*exp((-4000.0/(double)nombre_mot_dico)*(*(rang_min_pour_un_mot_cle+m)));
    double r=-d/(-2.0+(*(fitnessde_rang_min_pour_un_mot_cle+m)));
    nbre_point_total+=r;
    if(DEBUG){
      printf("\nrole rang:%f\ndans l'expon:%f\nnbre mot donne:%d\nfitnessutile:%f\naugmentation resultat:%f\n",
	     d,
	     (-4000.0/(double)nombre_mot_dico)*(*(rang_min_pour_un_mot_cle+m)),nombre_mot_donne,(*(fitnessde_rang_min_pour_un_mot_cle+m)),r);
      
    }
  }
  
  free(rang_min_pour_un_mot_cle);
  pretraitement_liberation_de_l_espace(adresse_offset_mot_cle,adresse_mot_cle,nombre_mot_cle,poids,adresse_rang,adresse_fitness,nombre_mot_donne);

   return nbre_point_total;
}
