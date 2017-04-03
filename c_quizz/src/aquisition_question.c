 /*
 *aquisition_question.c
 *Antecedent: nom_fichier_question pointe sur une chaine de caractere representant le nom d'un fichier de question valide
 *            retour est pointeur vers un tableau de 2 pointeur vers char
 *            numero_question est inferieur au nbre de question -1.superieur ou egal a 0.
 *Consequent:retour un tableau de 2 chaines de caractere 1 question 2 reponse
 *
 *@author:DUFRAIGNE ZANON 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
extern int aquisition_question(int numero_question,char *nom_fichier_question,char **retour){
/* SI le numero question est -1 ou n'est pas valide alors tirage au hazard sinon la question renvoye est celle demandee.*/
/* La graine vaut un chiffre positif si elle doit etre utiliser pour initialiser la serie aleatoire sinon elle vaut -1 cela doit se faire une fois seulement*/
  
  FILE *fp;
  int nbre_de_question=0;
  int tmp_offset_question=0;
  int tmp_offset_questionsuivante=0;
  int t=0;
  fp=fopen(nom_fichier_question,"r");
  /**/
  fread(&nbre_de_question,sizeof(int),1,fp);
  /**/
  int n=0;
  
  /*
   *Si le numero question est -1 ou n'est pas valide alors tirage au hazard sinon la question renvoye est celle demandee.*/
  if(numero_question==-1 || numero_question>=nbre_de_question || numero_question<0){
    numero_question=(int) (nbre_de_question*rand()/(RAND_MAX+1.0));
  }
  for(;n<numero_question;n++){
    /**/
    fread(&t,sizeof(int),1,fp);
    /**/
  }
  /**/
  fread(&tmp_offset_question,sizeof(int),1,fp);
  fread(&tmp_offset_questionsuivante,sizeof(int),1,fp);
  /**/
  
  *retour=malloc(tmp_offset_questionsuivante-tmp_offset_question);
  /*copie dans la memoire les chaines de caratere de la question est de la reponse*/
  char *tmp=*retour;
  fseek(fp,tmp_offset_question,SEEK_SET);
  int c;
  while((c=getc(fp))!='\n'){
    *tmp=c;
    tmp++;
  }
  *tmp='\0';
  tmp++;
  *(retour+1)=tmp;
  while((c=getc(fp))!='\n'){
    *tmp=c;
    tmp++;
  }
  *tmp='\0';
  fclose(fp);
  /*retour possede bien 2 pointeurs vers les chaines copiees.*/
  return nbre_de_question;
}


