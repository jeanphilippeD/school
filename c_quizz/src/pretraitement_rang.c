/*
 *Permet l'ajout du rang reel,evite les pb si 2 fitness identiques
 *
 *NE FONCTIONNE PAS SI 0 MOTS.
 *Antecedent:char **mots_du_dico,double *fitness_du_mot sont trié par ordre decroissant
 *Consequent les rang sont bons
 *
 *@author:DUFRAIGNE ZANON 
 */
#include <stdlib.h> 
#include <stdio.h>
#include "constante.h"


extern int* pretraitement_rang(int nbre_mot,double *fitness_du_mot){
  int *rang=(int*)calloc(nbre_mot,sizeof(int));
  int *retour_rang=rang;
   
  int n;
  int rang_tmp_de_premiere_egalite=0;/*permet d'eviter les ecart qui de proche en proche depasseraient epsilon*/
  int rang_tmp=0;
  *rang=rang_tmp;
  for(n=1;n<nbre_mot;n++){
    if((*(fitness_du_mot+rang_tmp_de_premiere_egalite)-*(fitness_du_mot+n))>epsilon || (n-rang_tmp_de_premiere_egalite)>Nombre_Max_de_Rang_Identiques){/*Si les fitness actuel est bien different du fitness precedent*/
      rang_tmp++;
      rang_tmp_de_premiere_egalite=n;
    }
    rang++;
    *rang=rang_tmp;
  }
  return retour_rang;/*pointeur vers la zone ou sont ranges les rangs*/
}

extern void pretraitement_rang_liberation_de_l_espace(int *rang){
  free(rang);
}
