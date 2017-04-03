/*
 *effectue tout le pretraitement pour un mot donne.
 *
 *@author:DUFRAIGNE ZANON 
 *
 *
 *
 *
 *Antecedent:double *fitness,int *rang ne sont pas initialises.
 *           char *mot_ref est un mot de la reponse donnee
 *           int nbremot est le nombre de mot du dico
 *           char **mot_du_dico pointe sur des pointeur des mot du dico.
 *
 *Consequent:mot_du_dicoson trie par ordre decroissant de fitness en reference a mot_ref.
 *           fitness et rang ont un espace alloue et possedent leur valeur sachant qu'il sont tries par ordre decroissant.
 *
 *
 */
#include <stdlib.h>
#include "pretraitement_fitness.h"
#include "pretraitement_tri.h"
#include "pretraitement_rang.h"
#include <stdio.h>
#include <math.h>
#include "constante.h"

void pretraitement_un_mot_pour_mot_dico(char*,int,char**,double**,int**);
void pretraitement_un_mot_pour_mot_cle(char*,int,char**,double**);
int recherche_non_dicotomique(double,int,double*,int*);

/*
 *Permet d'effectuer l'ennsemble des pretraitement:
 *      rend les fitness et les rangs associes aux mot cle pour un mot reference donne
 *
 *L'espace memoire pour le retour sera initialise a l'interieur de la methode et l'eventuelle refference de ces 2 pointeurs sera perdue
 *
 *
 *Donnees:
 *mot_ref est la chaine representant le mot donne
 *nbremot_cle est le nbre de mot cle de la reponse theorique
 *mot_cle est un pointeur sur les mots cles(pointeur sur pointeur de char)
 *nbremot_dico est le nbre de mot du dico
 *mot_du_dico est un pointeur sur les mots du dico(pointeur sur pointeur de char)
 *
 *Retour:
 *fitness est un pointeur sur les fitness des mots cles(pointeur sur pointeur de double)
 *rang est un pointeur sur les rang des mots cles(pointeur sur pointeur de int)
 *mot_cle est trie par ordre de fitness decroissant
 */
extern void pretraitement_un_mot(char *mot_ref,int nbremot_dico,char **mot_du_dico,int nbremot_cle,char **mot_cle,double **fitness,int **rang){
  double *fitness_pour_dico;
  int *rang_pour_dico;
  int n;
  pretraitement_un_mot_pour_mot_dico(mot_ref,nbremot_dico,mot_du_dico,&fitness_pour_dico,&rang_pour_dico);
  /*On a les fitness de tous les mots du dico*/
  pretraitement_un_mot_pour_mot_cle(mot_ref,nbremot_cle,mot_cle,fitness);
  /*On a les fitness de tous les mots cles*/
  if(DEBUG){
    printf("le mot:%s\n",mot_ref);
    for(n=0;n<5;n++){
      printf("mot:%s fit:%f rang:%d\n",*(mot_du_dico+n),*(fitness_pour_dico+n),*(rang_pour_dico+n));
    }
    printf("mot cle\n");
    for(n=0;n<nbremot_cle;n++){
      printf("not:%s fit:%f\n",*(mot_cle+n),*(*fitness+n));
    }
  }
  /*On va maintenant rechercher les rang des mots cle*/
  /* la liste des mots cle etant trie on peut au mot cle n se limiter a la recherche dans la zone n-1 -> FIN*/
  /* Il n'y a pas de probleme puisque deux mot de meme fitness ont meme raang*/
  

  int nbremot_dico_restant=nbremot_dico;
  double *fitness_pour_dico_restant=fitness_pour_dico;
  int *rang_pour_dico_restant=rang_pour_dico;
  int retour_recheche;
  
  *rang=(int*)calloc(nbremot_cle,sizeof(int));
  for(n=0;n<nbremot_cle;n++){
    retour_recheche=recherche_non_dicotomique(*((*fitness)+n),nbremot_dico_restant,fitness_pour_dico_restant,rang_pour_dico_restant);
    /*enregistrement du rang reel*/
    *((*rang)+n)=*(rang_pour_dico_restant+retour_recheche);
    /*Fin Enregistrement*/
    
    /*On decale la zone de recherche pour les suivants(elle conserve l'element trouve*/
    
    /*Optimisation posible:
     *nbremot_dico_restant-=retour_recheche;
     *fitness_pour_dico_restant+=retour_recheche;
     *rang_pour_dico_restant+=retour_recheche;
     */
    /*La zone est decallee*/
  }
  /*Les rang des mots cles sont affectes et il reste tries*/
  pretraitement_fitness_liberation_de_l_espace(fitness_pour_dico);
  pretraitement_rang_liberation_de_l_espace(rang_pour_dico);
}

void pretraitement_un_mot_pour_mot_dico(char *mot_ref,int nbremot,char **mot_du_dico,double **fitness,int **rang){
  /*Initialisation du fitness*/
  *fitness=pretraitement_fitness(mot_ref,nbremot,mot_du_dico);
  /*Fitness OK*/
  
  /*Tri des mot du dico et de lur fitness par ordre decroissant*/
  tri_par_fitness_croissant(nbremot,mot_du_dico,*fitness);
  /*tri par fitness OK*/
  
  /*intitialisation des rang*/
  *rang=pretraitement_rang(nbremot,*fitness);
  /*ajout rang OK*/
   
  
}
void pretraitement_un_mot_pour_mot_cle(char *mot_ref,int nbremot,char **mot_cle,double **fitness){
  /*Initialisation du fitness*/
  *fitness=pretraitement_fitness(mot_ref,nbremot,mot_cle);
  /*Fitness OK*/
  /*Tri des mot du dico et de lur fitness par ordre decroissant*/
  tri_par_fitness_croissant(nbremot,mot_cle,*fitness);
  /*tri par fitness OK*/
}



/*
 *rend le rang de meme fitness
 *Antecedent les mots cles sont dans le dictionnaire
 *rend la position dans la zone de recherche du 1er fitness equivalent trouve
 */
int recherche_non_dicotomique(double fitness_mot_cle,int nbremot_dico,double *fitness_pour_dico,int *rang_pour_dico){
  int n=0;
    

  /*
   *POURRAIT ETRE SOLIDIFIER  EN VERIFIANT QUE L'ON NE DEPASSE PAS
   *INUTILE SI L'ON SUIT BIEN L'ANTECEDANT:LES MOTS CLES SONT DANS LE DICO ET ORTOGRAPHIER DE LA MEME FACON
   *
   */
  while(n<nbremot_dico && fabs(fitness_mot_cle-*(fitness_pour_dico+n))>epsilon2){
    n++;
    
  }
  if(n==nbremot_dico){
    /*Fichier de question ou dico corompu
     */
    printf("Fichier de question ou dico corompu ");
    exit(1);
  }
    
   return n;
}

extern void pretraitement_un_mot_liberation_de_l_espace(int *rang,double *fitness){
  free(rang);
  pretraitement_fitness_liberation_de_l_espace(fitness);
}
