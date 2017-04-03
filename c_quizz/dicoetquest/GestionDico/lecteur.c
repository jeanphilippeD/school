/*
 *lecteur.c
 *Antecedent:le fichier de question est bien formater:
 *           question,RC,reponese('mot cle' poid 'mot cle'...),RC,quest...
 *           Le retour chariot est necessaire pour la derniere reponse
 *Consequent:Lecture sur l'entree standard
 *
 *@author:DUFRAIGNE ZANON 
 */
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{  
  
  FILE *fp;
  int nbre_de_mots=0;
  int espace_neccesaires_pour_les_mots=0;
  char *espace_des_mots;
  char **pointeur_sur_mots;
  char **tmp;
  int c;
  fp=fopen(argv[1],"r");
  
  fread(&nbre_de_mots,sizeof(int),1,fp);
  fread(&espace_neccesaires_pour_les_mots,sizeof(int),1,fp);
  printf("nbre de mots %d  taille %d \n",nbre_de_mots,espace_neccesaires_pour_les_mots);
  
  espace_des_mots=(char*)malloc(sizeof(char)*espace_neccesaires_pour_les_mots);
  pointeur_sur_mots=(char**)malloc(sizeof(char*)*nbre_de_mots);
  tmp=pointeur_sur_mots;/*Il ne faut pas detruire pointeur_sur_mots qui est la reference a la zone de mots*/
  
  while((c=fgetc(fp))!=EOF){
    ungetc(c,fp);
    *(tmp++)=espace_des_mots;/* on peut detruire le pointeur espace_des_mots.*/
    while((c=fgetc(fp))!='\n'){
      *(espace_des_mots++)=c;
    }
    *(espace_des_mots++)='\0';
  }
  /* toute la zone contient le mot et les pointeurs sont bons.*/
  int n;
  for(n=0;n<nbre_de_mots;n++){
    /**/
    printf("fin %d %s\n",n,*(pointeur_sur_mots+n));
  }
  fclose(fp);
  return 0;
}
