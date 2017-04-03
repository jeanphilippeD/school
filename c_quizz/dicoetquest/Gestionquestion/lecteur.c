/*
 *lecteur.c
 *Antecedent:le fichier de question est bien formater:
 *           question,RC,reponese('mot cle' poid 'mot cle'...),RC,quest...
 *           Le retour chariot est necessaire pour la derniere reponse
 *Consequent:La lecture est sur la sortie standard
 *
 *@author:DUFRAIGNE ZANON 
 */
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{  
  
  FILE *fp;
  int nbre_de_question=0;
  int tmp_offset_question=0;
  int c;
  fp=fopen(argv[1],"r");
  fread(&nbre_de_question,sizeof(int),1,fp);
  printf("nbre de question %d\n",nbre_de_question);
  int n=0;
  for(;n<nbre_de_question;n++){
    /**/
    fread(&tmp_offset_question,sizeof(int),1,fp);
    printf("question %d a l'offset %d\n",n,tmp_offset_question);
  }
  fread(&tmp_offset_question,sizeof(int),1,fp);
  printf("fin du fichier a l'offset %d\n",tmp_offset_question);
  printf("LES QUESTION");
  while((c=getc(fp))!=EOF){
    putchar(c);
  }
  fclose(fp);
  return 0;
}
