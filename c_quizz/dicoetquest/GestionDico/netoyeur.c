 
/*
 *Enleve les '\r' de Windows
 *
 *@author:DUFRAIGNE ZANON 
 */
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{  
  
  FILE *fp;
  FILE *fpdest;
  
  int c;
  printf("%d",argc);
  printf("%s  %s",argv[1],argv[2]);
  fp=fopen(argv[1],"r");
  fpdest=fopen(argv[2],"w");
  /* Sinon il y a bien des mots*/
  printf("on y va pour la recopie\n");
  
  /* Cette partie de code pourra etre reecrite avec fread et fwrite */
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
