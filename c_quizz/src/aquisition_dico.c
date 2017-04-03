/*
 *aquisition_dico.c
 *Antecedent:le fichier de question est bien formater:
 *           Le retour chariot est necessaire pour la derniere reponse
 *Consequent:pointeur_sur_pointeur_pour_mots_dico permetra l'acces direct au mot du dico qu se trouveront alors dans une zone reservee.
 *
 *@author:DUFRAIGNE ZANON 
 */
#include <stdio.h>
#include <stdlib.h>


extern void aquisition_dico(char *nom_fichier_dico,char ***pointeur_sur_pointeur_pour_mots_dico,int *nbmot)
{  
  
  FILE *fp;
  int nbre_de_mots=0;
  int espace_neccesaires_pour_les_mots=0;
  char *espace_des_mots;
  char **pointeur_sur_mots;
  char **tmp;
  int c;
  fp=fopen(nom_fichier_dico,"r");
  
  fread(&nbre_de_mots,sizeof(int),1,fp);
  pointeur_sur_mots=(char**)calloc(nbre_de_mots,sizeof(char*));
  tmp=pointeur_sur_mots;/*Il ne faut pas detruire pointeur_sur_mots qui est la reference a la zone de mots*/
  fread(&espace_neccesaires_pour_les_mots,sizeof(int),1,fp);
  espace_des_mots=(char*)calloc(espace_neccesaires_pour_les_mots,sizeof(char));  
  
  while((c=fgetc(fp))!=EOF){
    ungetc(c,fp);
    *(tmp++)=espace_des_mots;/* on peut detruire le pointeur espace_des_mots.*/
    while((c=fgetc(fp))!='\n'){
      *(espace_des_mots++)=c;
    }
    *(espace_des_mots++)='\0';
  }
  fclose(fp);
  if(((*pointeur_sur_mots)-espace_des_mots)>espace_neccesaires_pour_les_mots || ((tmp-pointeur_sur_mots)>nbre_de_mots)){
    printf("Depasement zone %d %d %d %d",((*pointeur_sur_mots)-espace_des_mots),espace_neccesaires_pour_les_mots,(tmp-pointeur_sur_mots),nbre_de_mots);
    exit(1);
  }
   
  /* toute la zone contient le mot et les pointeurs sont bons.*/
  
  /*mises en places des données de retour*/
  *pointeur_sur_pointeur_pour_mots_dico=pointeur_sur_mots;
  *nbmot=nbre_de_mots;
}
extern void aquisition_dico_liberation_de_l_espace(char **pointeur_sur_mots){
  free(*pointeur_sur_mots);/*libere espace_des_mots*/
  free(pointeur_sur_mots);
}
