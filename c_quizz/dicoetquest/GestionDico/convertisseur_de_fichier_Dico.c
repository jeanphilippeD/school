/*
 *convertisseur_de_fichier_question.c
 *Antecedent:le fichier dico est bien formater:
 *           les mots sont inscrits un mot par ligne
 *           Le retour chariot est necessaire pour la derniere reponse
 *           convertisseur_de_fichier_question source destination.
 *
 *           le ÿ est INTERDIT.non mais hhhe(=EOF)
 *Consequent:Le fichier dest est utilisable par le programme
 *
 *@author:DUFRAIGNE ZANON 
 */
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{  
  
  FILE *fp;
  FILE *fpdest;
  int nbre_de_mots=0;
  int espace_neccesaires_pour_les_mots=0;
  int c;
  printf("%d",argc);
  printf("%s  %s",argv[1],argv[2]);
  fp=fopen(argv[1],"r");
  fpdest=fopen(argv[2],"w");
  while((c=fgetc(fp))!=EOF){
    ungetc(c,fp);
    while((c=fgetc(fp))!='\n' && c!='\r'){
      espace_neccesaires_pour_les_mots++;/*on compte tous les caracteres*/
    }
     /*un mot a ete parcourue*/
    nbre_de_mots++;
    espace_neccesaires_pour_les_mots++;/*on compte le '\n' a rajouter*/
    printf("%d\n",nbre_de_mots);
    
    
    /* elimination des '\r' SUPERFLUX*/
    if(c=='\r'){
      c=fgetc(fp);/* on l'elimine du compte le '\r'=13=cr qui sert a rien*/
    }
    /*sinon c'est un '\n' et on le compte*/
    else{/* mais on verifie que le '\r' n'est pas apres*/
      c=fgetc(fp);
      if(c!='\r'){/*il n etait pas la*/
	ungetc(c,fp);
      }/*Sinon BOOOOOOOOOO WINDOWS et donc Rien*/
    }
    
    /*FIN elimination des '\r' SUPERFLUX*/
  }
  /*nbre_de_mots est bon*/
  if(nbre_de_mots==0){
    printf("il n'y a pas de mots dans le fichier");
    return 1;
  }
  /* Sinon il y a bien des mots*/
  printf("on y va pour la recopie\n");
  
  /* Cette partie de code pourra etre reecrite avec fread et fwrite */
  fwrite(&nbre_de_mots,sizeof(int),1,fpdest);
  fwrite(&espace_neccesaires_pour_les_mots,sizeof(int),1,fpdest);
  /**/
  /*tout les entier sont inscrit il n'y a plus qua recopier les question et reponses.*/
  rewind(fp);/* retour en debut de fichier pour la recopie*/
  
  while((c=fgetc(fp))!=EOF){
    ungetc(c,fp);
    while((c=fgetc(fp))!='\n' && c!='\r'){
      fputc(c,fpdest);/* on met tous les caracteres*/
    }
    /*un mot a ete parcourue*/
    
    /* elimination des '\r' SUPERFLUX*/
    if(c=='\r'){
      c=fgetc(fp);/* on l'elimine du compte le '\r'=13=cr qui sert a rien*/
    }
    /*sinon c'est un '\n' et on le compte*/
    else{/* mais on verifie que le '\r' n'est pas apres*/
      c=fgetc(fp);
      if(c!='\r'){/*il n etait pas la*/
	ungetc(c,fp);
      }
    }
    fputc('\n',fpdest);/*ajout du caractere de LF*/
    /*FIN elimination des '\r' SUPERFLUX*/
  }
  fclose(fp);
  fclose(fpdest);
  /*printf("EOF = %d %c",EOF,EOF);*/
  return 0;
}
