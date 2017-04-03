/*
 *convertisseur_de_fichier_question.c
 *Antecedent:le fichier de question est bien formater:
 *           question,RC,reponese('mot cle' poid 'mot cle'...),RC,quest...
 *           Le retour chariot est necessaire pour la derniere reponse
 *           convertisseur_de_fichier_question source destination.
 *Consequent:Le fichier de question est utilisable par le programe
 *
 *@author:DUFRAIGNE ZANON 
 */
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{  
  
  FILE *fp;
  FILE *fpdest;
  int nbre_de_question=0;
  int tmp_offset_question=0;
  int c;
  int offset_decalage_pour_saut_entier=0;/*ajoute aux offset pour compenser l'ajout des entier au debut du fichier*/
  printf("%d",argc);
  printf("%s  %s",argv[1],argv[2]);
  fp=fopen(argv[1],"r");
  fpdest=fopen(argv[2],"w");
  while((c=fgetc(fp))!=EOF){
    ungetc(c,fp);
    while((c=fgetc(fp))!='\n'){
    }
    /*la question a ete parcourue*/
    while((c=fgetc(fp))!='\n'){
    }
    /*la reponse a ete parcourue*/
    nbre_de_question++;
  }
  /*nbre_de_question est bon*/
  if(nbre_de_question==0){
    printf("il n'y a pas de question dans le fichier");
    return 1;
  }
  /* Sinon il y a bien des question*/
  
  
  fwrite(&nbre_de_question,sizeof(int),1,fpdest);
  /**/
  rewind(fp);/* retour en debut de fichier*/


  /*
   *il faut ajouter a l'offset indiquant le decalage par raport au debut de la preiere question la valeur prise par les nombres avant
   *Utilisation future de int fseek (FILE *stream, long offset,SEEK_SET);
   */
  offset_decalage_pour_saut_entier=(nbre_de_question+2)*sizeof(int);
  tmp_offset_question=offset_decalage_pour_saut_entier;
  
  int n=0;
  for(;n<nbre_de_question;n++){
    /**/
    fwrite(&tmp_offset_question,sizeof(int),1,fpdest);
    /**/
    while((c=fgetc(fp))!='\n'){
      tmp_offset_question++;
    }
    tmp_offset_question++;
    /*la question a ete parcourue*/
    while((c=fgetc(fp))!='\n'){
      tmp_offset_question++;
    }
    tmp_offset_question++;
    /*la reponse a ete parcourue*/
    /*tmp_offset_question permet l'acces a la question suivante*/
  }
  /*On ajoute aussi l'offset de fin de fichier*/
  fwrite(&tmp_offset_question,sizeof(int),1,fpdest);
  
  /*tout les entier sont inscrit il n'y a plus qua recopier les question et reponses.*/
  rewind(fp);/* retour en debut de fichier*/
  
  while((c=fgetc(fp))!=EOF){
    fputc(c,fpdest);
  }
  fclose(fp);
  fclose(fpdest);
  return 0;
}
