 /*
 *pretraitement_conversion.c
 *Permet de convertir la reponse donne et la reponse theorique en donnee exploitable directement
 *
 *@author:DUFRAIGNE ZANON 
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int pretraite_nbre_mot_cle(const char *reponse_theorique){
  int retour=0;
  const char *tmp=reponse_theorique;
  while(*tmp!='\0'){
    while(*tmp!=' '){
      tmp++;
    }
    tmp++;
    /*mot cle passe*/
    while(*tmp!=' ' && *tmp!='\0'){
      tmp++;
    }
    if(*tmp==' ') tmp++;
    /*poid passe*/
    retour++;
  }
  return retour;
}
int pretraite_nbre_mot_donne(const char *reponse_donnee){
  int retour=0;
  const char *tmp=reponse_donnee;
  while(*tmp!='\0'){
    while(*tmp==' '){/*elimination caractere superflux*/
      tmp++;
    }
    if(*tmp=='\0')
      return retour;/*ya plus rien apres*/
    while(*tmp!=' ' && *tmp!='\0'){
      tmp++;
    }
    /*mot donne passe*/
    if(*tmp==' '){
      while(*tmp==' '){/*elimination caractere superflux*/
	tmp++;
      }
    }
    retour++;
  }
 return retour;
}

  
/*adresse_mot_cle poite sur le poiteur(espace memoire) qui poite sur le mot cle*/
/* les poid seront au maximum de int signes */
int pretraite_mot_cle_et_poid(const char *reponse_theorique,int nbre_mot_cle,char **adresse_mot_cle,int *poid_mot_cle){
  const char *tmp=reponse_theorique;
  const char *tmp2;
  char *tmp3;
  int taille_mot_cle;
  char BUFFER_POID[12];/* taille necessaire pour un int signe*/
  char *tmpbuf;
  int buffertest;
  while(*tmp!='\0'){
    tmp2=tmp;/* sauvegarde debut du mot */
    taille_mot_cle=0;
     while(*tmp!=' '){
      tmp++;
      taille_mot_cle++;
    }
    tmp++;
    taille_mot_cle++;
    
    /*mot cle passe on a la taille de la memoire necessaire pour le mot cle*/
    *adresse_mot_cle=malloc(sizeof(char)*taille_mot_cle);/*on reserve cet espace*/
    tmp3=*adresse_mot_cle;
    /* on remplie cet espace avec le mot cle*/
    while(*tmp2!=' '){
      *tmp3=*tmp2;
      tmp2++;
      tmp3++;/*sizeof(char)*/
    }
    *tmp3='\0';/* le mot cle est a present une chaine normalisee */
    adresse_mot_cle++;/* a la sortie de la methode cette image aura disparue et le pointeur transmis pointera sur le debut de la zone de pointeur */
    
    /*mot cle passe*/

    tmpbuf=BUFFER_POID;
    buffertest=0;/* permet d'empecher le depassement de la capacite du BUFFER de maniere grossiere.*/
    while(*tmp!=' ' && *tmp!='\0'){
      if(buffertest<11){
	*tmpbuf=*tmp;
	tmpbuf++;
	buffertest++;
      }
      tmp++;
    }
    *tmpbuf='\0';
    *poid_mot_cle=atoi(BUFFER_POID);
    poid_mot_cle++;
    
    if(*tmp==' ') tmp++;
    /*poid passe*/
    /*  une case de poid_mot_cle initialisee.*/
  }
  return 0;
}



/*adresse_mot_cle poite sur le poiteur(espace memoire) qui poite sur le mot cle*/
/* les poid seront au maximum de int signes */
int pretraite_mot_donne(const char *reponse_donne,int nbre_mot_donne,char **adresse_mot_donne){
  const char *tmp=reponse_donne;
  const char *tmp2;
  char *tmp3;
  int taille_mot_donne;
  while(*tmp!='\0'){
     while(*tmp==' '){/*elimination caractere superflux*/
       tmp++;
     }
     tmp2=tmp;/* sauvegarde debut du mot */
     taille_mot_donne=0;
     while(*tmp!=' ' && *tmp!='\0'){
       tmp++;
       taille_mot_donne++;
     }
     if(*tmp==' '){
       while(*tmp==' '){/*elimination caractere superflux*/
	 tmp++;
       }
     }
     taille_mot_donne++;
    
     /*mot cle passe on a la taille de la memoire necessaire pour le mot cle*/
     *adresse_mot_donne=malloc(sizeof(char)*taille_mot_donne);/*on reserve cet espace*/
     tmp3=*adresse_mot_donne;
     /* on remplie cet espace avec le mot cle*/
     
     while(*tmp2!=' ' && *tmp2!='\0'){
       *tmp3=*tmp2;
       tmp2++;
       tmp3++;/*sizeof(char)*/
     }
     *tmp3='\0';/* le mot cle est a present une chaine normalisee */
     adresse_mot_donne++;/* a la sortie de la methode cette image aura disparue et le pointeur transmis pointera sur le debut de la zone de pointeur */
     /*mot cle passe*/
  }
  return 0;
}


/*
 *Utiliser Avant le traitement de la reponse
 *reponse_theorique est la chaine de caractere representant la reponse theorique(cad un pointeur sur son 1er caractere).
 *La reponse theorique etant formatee tel que suit:"MOTCLE1 POID1 MOTCLE2 POID2 ... MOTCLEN POIDN"
 * ex:"VICTOR 3 HUGO 7".
 *
 *reponse_donne est la chaine de caractere representant la reponse donnee(cad un pointeur sur son 1er caractere).
 * ex:"VICTOR HUGO"
 *
 *adresse_mot_cle est un pointeur(qui pointe vers un) de pointeur de pointeur de char qui permettera dutiliser a la sortie de la fonction le pointeur *adresse_mot_cle direcement comme si c'etait un tableau de pointeur vers les mots cles. (tableau de taille *nombre_mot_th)
 *
 *poids est un pointeur de pointeur d'entier qui permettera dutiliser a la sortie de la fonction le pointeur *poids comme si c'etait un tableau d'entier. (tableau de taille *nombre_mot_th)
 *
 *adresse_mot_donne  est un pointeur(qui pointe vers un) de pointeur de pointeur de char qui permettera dutiliser a la sortie de la fonction le pointeur *adresse_mot_donne direcement comme si c'etait un tableau de pointeur vers les mots donnes. (tableau de taille *nombre_mot_donne)
 *
 *nombre_mot_th est un pointeur d'entier qui permettera d'utiliser a la sortie de la fonction l'entier *nombre_mot_th pour connaitre la taille du tableau (fictif) fait.
 *
 *nombre_mot_donne est un pointeur d'entier qui permettera d'utiliser a la sortie de la fonction l'entier *nombre_mot_donne pour connaitre la taille du tableau (fictif) fait.
 *
 */
extern void pretraitement_conversion(const char *reponse_theorique,const char *reponse_donne,char ***adresse_mot_cle,int **poids,char ***adresse_mot_donne,int *nombre_mot_th,int *nombre_mot_donne){
  int nbre_mot_th=pretraite_nbre_mot_cle(reponse_theorique);
  int nbre_mot_don=pretraite_nbre_mot_donne(reponse_donne);
  *nombre_mot_th=nbre_mot_th;
  *nombre_mot_donne=nbre_mot_don;
 
  *adresse_mot_cle=(char**)malloc(nbre_mot_th*sizeof(int));
  *poids=malloc(nbre_mot_th*sizeof(int));
  *adresse_mot_donne=(char**)malloc(nbre_mot_don*sizeof(int));

  pretraite_mot_cle_et_poid(reponse_theorique,nbre_mot_th,*adresse_mot_cle,*poids);
  pretraite_mot_donne(reponse_donne,nbre_mot_don,*adresse_mot_donne);
}

extern void pretraitement_conversion_liberation_d_espace_mots(int nombre_mot,char **adresse_mot){
  int i;
  for(i=0;i<nombre_mot;i++){
    free(*(adresse_mot+i));
  }
  free(adresse_mot);
}
extern void pretraitement_conversion_liberation_d_espace_poids(int *poids){
  free(poids);
}
