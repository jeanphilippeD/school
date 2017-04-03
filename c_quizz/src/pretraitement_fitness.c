
/*
 *Permet l'ajout du fitness a tout les mots du dico
 *
 *
 *mot_reference a partir duquel le fitness doit etre calculer
 *nbre_mot est la taille du tableau de pointeur vers les mots du dico
 *mot_du_dicotableau tableau qui contient les pointeur vers les mots du dico(sous forme de chaine standard:"mot\0") et de taille nbre_mot
 *de plus les mot du dico on ete mis en memoire dans une zone unique et le mot suivant est pointe par le pointeur suivant(permet de determiner rapidement la taille d'un mot)
 *fitness_du_mot tableau qui contient les fitness des mots du dico et de taille nbre_mot(memoire prereservee)
 *
 *Remarque le traitement se fait ici sur des MINUSCULES et des LETTRES (pas de chiffre traitment ailleur).
 *
 *
 *@author:DUFRAIGNE ZANON 
 */


#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <limits.h>
#include "constante.h"



void replissage_lettre_des_mots(char*,int,int[taille_table_de_caracteres]);
void lettres_presentent_manquante_entrop(int[taille_table_de_caracteres],int[taille_table_de_caracteres],int*,int*,int*);
void calcul_ecart_relatif(int,char*,int,char*,const int[taille_table_de_caracteres],const int[taille_table_de_caracteres],double*);
void elimination_des_problemes_de_calcul_d_ecart(int,char*,int,char*,const int[taille_table_de_caracteres],const int[taille_table_de_caracteres],double*);
double fctsomme_Vabsolue_ecart_relatif(double*,int);
double fctsomme_Vrelative_ecart_relatif(double*,int);
double fctmoyenne_ecart(double*,int);
double fctsomme_ecart_relatif_aucarre(double*,int);
double fctmoyenne_ecart_carre(double*,int);
double fctvariance_ecart(double*,int,double);
double fctvariance_normalisee(double*,int ,double);
double calcul_du_fitness(double,double,double,double,double);
double calcul_du_fitness_normalise(int,int,int,int,int,double,double);

/*normalement les approx seront a epsilon pres*/
int approche(double d){
  int dessous=floor(d);
  if(d<dessous+0.5)
    return dessous;
  else
    return dessous+1;
}
extern double* pretraitement_fitness(char *mot_reference,int nbre_mot,char **mots_du_dico){
   double *fitness_du_mot=calloc(nbre_mot,sizeof(double));/*retour*/
  /*
   *POUR EVITER TOUTE CONVERSION EXPLICITE QUI RENDENT LE CODE MOINS CLAIR
   *ET POUR AVOIR UNE DIVISION AVEC NOMBRE A VIRGULE TOUTE LES VALEURS 
   *UTILISEES POUR LE CALCUL DU FITNESS SERONT DES DOUBLES.
   */
  int taille_mot_reference=strlen(mot_reference);
   int taille_mot_dico;
  int lettre_mot_reference[taille_table_de_caracteres];
  int lettre_mot_dico[taille_table_de_caracteres];
  
  /*ecart relatif en valeur absolue : intialise a -1 avant la fin initialisation normalises par la taille de l'element du dico*/
  double *ecart_relatif=calloc(taille_mot_reference,sizeof(double));
  
  double somme_ecart_relatif;
  double variance_ecart;
  double variance_normalisee;
  double moyenne_ecart;
  double moyenne_ecart_carre;
  
  
  char *mot_dico_tmp;/* pointeur direct vers le premier charactere du mot du dico voulu*/
  int lettres_presentent;
  int lettres_manquantent;
  int lettres_en_trop;
  int m=0;



  /*initialisation*/
  
  replissage_lettre_des_mots(mot_reference,taille_mot_reference,lettre_mot_reference);/*initialisation de lettre_mot_reference*/
  
  /*fin initialisation*/
  
  for(m=0;m<nbre_mot;m++){
    /*initialisation*/
    mot_dico_tmp=*(mots_du_dico+m);
    taille_mot_dico=strlen(mot_dico_tmp);/*taille_mot_dico=*(mots_du_dico+m)-*(mots_du_dico+1+m)-1 (sans le \0)*/
    
    replissage_lettre_des_mots(mot_dico_tmp,taille_mot_dico,lettre_mot_dico);/*initialisation de lettre_mot_dico*/
    
    /*lettre presente en trop manquante*/
    /*Mise en place de leur valeur reelle.*/
    lettres_presentent_manquante_entrop(lettre_mot_reference,lettre_mot_dico,&lettres_presentent,&lettres_manquantent,&lettres_en_trop);
    /*fin lettre presente manquante en trop*//*OK*/

    /*Calcul de la variance et ecart
     *il n'est pas important ici de verifier qu'on teste 2 fois la meme lettre
     *(pourra peut etre ameliorer si resultat decevant)
     */
    /*ECART*/
     calcul_ecart_relatif(taille_mot_reference,mot_reference,taille_mot_dico,mot_dico_tmp,lettre_mot_dico,lettre_mot_reference,ecart_relatif);
     
    /*Ecart relatif rempli*/
    
    /*Moyenne Ecart et Somme ecart relatif*/
    somme_ecart_relatif=fctsomme_Vabsolue_ecart_relatif(ecart_relatif,taille_mot_reference);
    moyenne_ecart=fctmoyenne_ecart(ecart_relatif,taille_mot_reference);
    moyenne_ecart_carre=fctmoyenne_ecart_carre(ecart_relatif,taille_mot_reference);


    /* fin moyenne */

    /*Variance de l'ecart: moyenne_ecart_carre-pow(moyenne_ecart,2);*/
    variance_ecart=fctvariance_ecart(ecart_relatif,taille_mot_reference,moyenne_ecart);
    variance_normalisee=fctvariance_normalisee(ecart_relatif,taille_mot_reference,moyenne_ecart);
   /* variance_ecart BON */
   
    
    
    /*Mise a jours du fitness*/
    *(fitness_du_mot+m)=calcul_du_fitness_normalise(taille_mot_dico,taille_mot_reference,lettres_presentent,lettres_manquantent,lettres_en_trop,variance_ecart,somme_ecart_relatif);
    
    
  }
  /*
   *
   *
   */
  free(ecart_relatif);
  /*
   *
   *
   */
  return fitness_du_mot;
}




void replissage_lettre_des_mots(char *mot,int taille_mot,int lettre[taille_table_de_caracteres]){
  int n;
  for(n=0;n<taille_table_de_caracteres;n++){/*initalisation de lettre a 0*/
    lettre[n]=0;
  }
  
  for(n=0;n<taille_mot;n++){/*pour tout les caractere du mot on augmente la case de lettrequi lui correspond de 1.*/
     lettre[*(mot+n)+type_of_caractere]++;
  }
}


void lettres_presentent_manquante_entrop(int lettre_mot_reference[taille_table_de_caracteres],int lettre_mot_dico[taille_table_de_caracteres],int *lettres_presentent,int *lettres_manquantent,int *lettres_en_trop){
  int n;
  int soustraction;
  *lettres_presentent=0;
  *lettres_manquantent=0;
  *lettres_en_trop=0;


  for(n=0;n<taille_table_de_caracteres;n++){
    soustraction=lettre_mot_dico[n]-lettre_mot_reference[n];
    /* les lettres presentes sont mis a jours a l'interieur de la condition:
       on prend le minimum de ces  2 nbre positifs*/
    
    if(0>soustraction){/*<=>(lettre_mot_reference[n]>lettre_mot_dico[n])*/
      /*il manque des lettre ds le mot du dico*/
      *lettres_manquantent-=soustraction;
      *lettres_presentent+=lettre_mot_dico[n];
    }
    else{
      /*il a des lettres en trop ds le mot du dico*/
      *lettres_en_trop+=soustraction;
      *lettres_presentent+=lettre_mot_reference[n];
    }
  }
}


/*
 *On peut remarquer ici que 
 *double *ecart_relatif est une copie et donc le modifier n'affectera pas le vrai
 */
void calcul_ecart_relatif(int taille_mot_reference,char *mot_reference,int taille_mot_dico,char *mot_dico,const int lettre_mot_dico[taille_table_de_caracteres],const int lettre_mot_reference[taille_table_de_caracteres],double *ecart_relatif){
  /*
   */
  

  char tmp;
  /* entier de deplacement de pointeur de caractere de parcourt des mot sens 1 lecture 2 retour*/
  int car1,car2;/* NE POSENT PAS DE PROBLEMES CAR SONT DIVISES PAR DES DOUBLE AVANT LE RETOUR*/
  /*
   */ 
  int n;
  
  
  for(n=0;n<taille_mot_reference;n++){/*initialisation a une valeur impossible de ecart_relatif*/
    *(ecart_relatif+n)=-2;
  }
  
  for(n=0;n<taille_mot_reference;n++){
    tmp=*(mot_reference+n);
    car1=1;/*inutile de tester 2* le caractere au meme endroit*/
    car2=0;
    if(lettre_mot_dico[tmp+type_of_caractere]>0){
      if(n>=taille_mot_dico-1){
	car2=(n-taille_mot_dico+1);
	while(*(ecart_relatif+n)==-2){
	  if(tmp==*(mot_dico+n-car2)){
	    *(ecart_relatif+n)=-car2/((double)taille_mot_dico);
	  }
	  car2++;
	}
      }
      else{
	/*
	 *La finitude de la boucle est assuree par le fait que la lettre en question est presente dans la mot du dico.
	 *Ce qui changera ecart relatif qui vaudra au minimum -1.
	 */
	while(*(ecart_relatif+n)==-2){/* le && implique que si 1ere condition fausse 2eme pas testee */
	  /* l'ordre du car1 avant car2 car car2 est plus petit donc il faut le garder si c'est lui*/
	  
	  if(car1 != -1){
	    if(tmp==*(mot_dico+n+car1)){
	      *(ecart_relatif+n)=car1/((double)taille_mot_dico);
	    }
	    car1++;
	    if(n+car1>taille_mot_dico-1){/*depasse par le haut*/
	      car1=-1;
	    } 
	  }
	  
	  if(car2 != -1){
	    if(tmp==*(mot_dico+n-car2)){
	      *(ecart_relatif+n)=-car2/((double)taille_mot_dico);
	    }
	    car2++;
	    if(n-car2 < 0){/*depasse par le bas*/
	      car2=-1;
	    }
	  }
	}
      }
    }
    /*
     *Sinon Rien la lettre n'est pas presente et n'intervient donc pas dans l'ecart relatif.
     */
        
  }
  /*ecartrelatif est bien repli cependant des lettre on ete utiliser plusieurs fois ce qui peut entrainer des defaut
   *on va repasser dessus et eliminer le pb.*/
  elimination_des_problemes_de_calcul_d_ecart(taille_mot_reference,mot_reference,taille_mot_dico,mot_dico,lettre_mot_dico,lettre_mot_reference,ecart_relatif);

}

/*
 * Pour ce faire on va garder les ecart minimum(en val absolue)(ceux qui on le + de chance d'etre vrai
 * et on va remplacer les autres par -2( lettre non trouvee)
 */
void elimination_des_problemes_de_calcul_d_ecart(int taille_mot_reference,char *mot_reference,int taille_mot_dico,char *mot_dico,const int lettre_mot_dico[taille_table_de_caracteres],const int lettre_mot_reference[taille_table_de_caracteres],double *ecart_relatif){
  int n,m,rang_de_min_trouve,tmp2,car1,car2;
  double min_trouve;
  char tmp;
  /*booleens*/
  int *ecart_valide=(int*)calloc(taille_mot_reference,sizeof(int));/* on y touchera plus apres */
  int *lettre_invalide=(int*)calloc(taille_mot_dico,sizeof(int));/* on y touchera plus apres */
  /**/
  int lettre_mot_dico_tmp[taille_table_de_caracteres];/*COPIE Modifiable de 'lettre_mot_dico'*/
  
  for(n=0;n<taille_table_de_caracteres;n++){
    lettre_mot_dico_tmp[n]=lettre_mot_dico[n];
  }
  for(n=0;n<taille_mot_reference;n++){
      *(ecart_valide+n)=0;/*indique Faux*/
  }
  for(n=0;n<taille_mot_dico;n++){
    *(lettre_invalide+n)=0;/*indique Faux*/
  }

  for(n=0;n<taille_mot_reference;n++){
     tmp=*(mot_reference+n);
    /*if(lettre_mot_dico_tmp[tmp+type_of_caractere]>=lettre_mot_reference[tmp+type_of_caractere])
     *PAS DE PB.
     * ou si  lettre_mot_dico_tmp[tmp+type_of_caractere]==0.pas de pb
     */
    if(lettre_mot_dico_tmp[tmp+type_of_caractere]>0)
      {
	if(lettre_mot_dico_tmp[tmp+type_of_caractere]<lettre_mot_reference[tmp+type_of_caractere]){
      /* presence de PB*/
	  while(lettre_mot_dico_tmp[tmp+type_of_caractere]>0){/* tant qu'on peut placer des lettres*/
	    
	    if(*(ecart_valide+n)==0){/* n'a pas deja ete utilise dans le processus de correction*/
	      min_trouve=*(ecart_relatif+n);
	      rang_de_min_trouve=n;
	    }
	    else{
	      min_trouve=INT_MAX;/* sera bligatoirement remplacer si le processus est bon*/
	      rang_de_min_trouve=0;/* fait pas planter si le processus ne marche pas*/
	      /*ET CA DOIT MARCHER*/
	    }
	    for(m=n+1;m<taille_mot_reference;m++){/* le premier ne sert a rien on compare avec lui meme*/
	      if(tmp==*(mot_reference+m)){/*on est sur une meme lettre donc on peut verifier */
		if(*(ecart_valide+m)==0){/* on n'est sur un ecart non encore valide*/
                  if(fabs(min_trouve)>= fabs(*(ecart_relatif+m))){/* si le maximum est plus loin on change (=pour le 0)*/
		    min_trouve=*(ecart_relatif+m);
		    rang_de_min_trouve=m;
                  }
		}
	      }
	    }
	    /* on a decouvert le minimum */
	    /* On le VALIDER*/
	    *(ecart_valide+rang_de_min_trouve)=1;/* il est valide*/
	    /* On invalide le caractere du mot_du_dico*/
	    /* Bizare sans la varible tmp2 il veut pas a la compilation sou win*/
	    tmp2=rang_de_min_trouve +  approche((*( ecart_relatif + rang_de_min_trouve ))*taille_mot_dico);/*On retrouve car*/
	    
	    if(!(tmp2<taille_mot_dico)){
	      tmp2=taille_mot_dico-1;
	      }
	    
	    
	    if(tmp2<0){
	      tmp2=0;
	    }
	    
	    *(lettre_invalide + tmp2)=1;/* il est invalide*/
	    /**/
	    lettre_mot_dico_tmp[tmp+type_of_caractere]--;/* il y a une lettre de mois disponible*/
	    /* lorsqu'il sera arrive a 0 on aura valider bon nombre d'ecart_relatif*/
	    
	    /*MAINTENANT IL FAUT RECALCULER LES ECART RELATIF QUI NE SONT PAS VALIDES POUR CETTE LETTRE (tmp)*/
	    /*SI IL RESTE DES LETRES DANS LE DICO SINON CA SERT A RIEN*/
	    if(lettre_mot_dico_tmp[tmp+type_of_caractere]>0){
	      for(m=n;m<taille_mot_reference;m++){/* le premier doit etre verifie*/
		if(tmp==*(mot_reference+m)){/*on est sur une meme lettre donc on peut verifier */
		  if(*(ecart_valide+m)==0){/* on n'est sur un ecart non encore valide*/
		    /*RECALCUL sur le meme principe que la methode du meme nom
		     *MAIS on tient compte des exceptions
		     */
		    car1=1;/*inutile de tester 2* le caractere au meme endroit*/
		    car2=0;
		    *(ecart_relatif+m)=-2;
		    /*Mise a jours de ecart_relatif*/
		    
		    if(m>=taille_mot_dico-1){
		      car2=(m-taille_mot_dico+1);
		      while(*(ecart_relatif+m)==-2){
			if(tmp==*(mot_dico+m-car2)){
			  if(*(lettre_invalide+m-car2)==0){/* non invalide */
			    *(ecart_relatif+m)=-car2/((double)taille_mot_dico);
			  }
			}
			car2++;
		      }
		    }
		    else{
		      /*
		       *La finitude de la boucle est assuree par le fait que la lettre en question est presente dans la mot du dico.
		       *Ce qui changera ecart relatif qui vaudra au minimum -1.
		       */
		      while(*(ecart_relatif+m)==-2){/* le && implique que si 1ere condition fausse 2eme pas testee */
			/* l'ordre du car1 avant car2 car car2 est plus petit donc il faut le garder si c'est lui*/
			if(car1 != -1){
			  if(tmp==*(mot_dico+m+car1)){
			    if(*(lettre_invalide+m+car1)==0){/* non invalide */
			      *(ecart_relatif+m)=car1/((double)taille_mot_dico);
			    }
			  }
			  car1++;
			  if(m+car1>taille_mot_dico-1){/*depasse par le haut*/
			    car1=-1;
			  } 
			}
			
			if(car2 != -1){
			  if(tmp==*(mot_dico+m-car2)){
			    if(*(lettre_invalide+m-car2)==0){/* non invalide */
			      *(ecart_relatif+m)=-car2/((double)taille_mot_dico);
			    }
			  }
			  car2++;
			  if(m-car2 < 0){/*depasse par le bas*/
			    car2=-1;
			  }
			}
		      }
		    }/*fin else*/
		    /*FIN DE CE CALCUL*/
		  }/*fin if*/
		}/*fin if*/
	      }/*fin du for m*/
	    }/* fin if letre>0 */
	    
	    /*FIN RECALCUL ECART_RELATIF*/
	    /* on peut recomencer si necessaire*/
	  
	  }/*fin while*/
	  /* Il ne reste plus que les dechets a enlever*/
	  for(m=n;m<taille_mot_reference;m++){/* le premier doit etre verifie*/
	    if(tmp==*(mot_reference+m)){/*on est sur une meme lettre donc on peut verifier */
	      if(*(ecart_valide+m)==0){/* on n'est sur un ecart non encore valide*/
		/* Il doit etre evacuer et donc remplacer par la valeur par deffaut*/
		*(ecart_relatif+m)=-2;
	      }
	    }
	  }/*fin for*/
	  /* Les dechet sont enleves les ecart pour cette lettre sont bon
	   * lettre_mot_dico_tmp[tmp+type_of_caractere]==0 d'ou on y reviendra plus*/
	
	}
      }
   }/*fin for*/
  
  free(ecart_valide);
  /*ecart_valide=NULL;*/
    
  free(lettre_invalide);
  /*lettre_invalide=NULL;*/
 
}

double fctsomme_Vabsolue_ecart_relatif(double *ecart_relatif,int taille_mot_reference){
  double somme_ecart_relatif=0;
  int n;
  
  for(n=0;n<taille_mot_reference;n++){
    somme_ecart_relatif+=(*(ecart_relatif+n) > -2 ? fabs(*(ecart_relatif+n)) : 1);
    /* -1<= *(ecart_relatif+n) <=1 si il est initialise sinon il vaut -2*/
  }
  return somme_ecart_relatif;
}

double fctsomme_Vrelative_ecart_relatif(double *ecart_relatif,int taille_mot_reference){
  double somme_ecart_relatif=0;
  int n;
  double somme_tmp=0;/*Si l'ecart relatif n'a pas ete initialise on lui ajoute 1(val abs Max)
		   *A la fin on l'ajoute a somme_ecart_relatif avec le signe 
		   *qui l'augmente le plus en valeur absolue.
		   */
  for(n=0;n<taille_mot_reference;n++){
    /* -1<= *(ecart_relatif+n) <=1 si il est initialise sinon il vaut -2*/
    if(*(ecart_relatif+n) > -2){
      somme_ecart_relatif+=*(ecart_relatif+n);
    }
    else somme_tmp++;
    
  }
  return (fabs(somme_ecart_relatif+somme_tmp)<fabs(somme_ecart_relatif-somme_tmp) ? somme_ecart_relatif-somme_tmp : somme_ecart_relatif+somme_tmp);
}

double fctmoyenne_ecart(double *ecart_relatif,int taille_mot_reference){
  
  return (fctsomme_Vrelative_ecart_relatif(ecart_relatif,taille_mot_reference)/((double)taille_mot_reference));
}

double fctsomme_ecart_relatif_aucarre(double *ecart_relatif,int taille_mot_reference){
  double somme_ecart_relatif_aucarre=0;
  int n;
  
  for(n=0;n<taille_mot_reference;n++){
    somme_ecart_relatif_aucarre+=pow((*(ecart_relatif+n) > -2 ? *(ecart_relatif+n) : 1),2);
    /* -1<= *(ecart_relatif+n) <=1 si il est initialise sinon il vaut -2*/
  }
  return somme_ecart_relatif_aucarre;
}

double fctmoyenne_ecart_carre(double *ecart_relatif,int taille_mot_reference){
  return (fctsomme_ecart_relatif_aucarre(ecart_relatif,taille_mot_reference)/((double)taille_mot_reference));
}


double fctvariance_ecart(double *ecart_relatif,int taille_mot_reference,double moyenne_ecart){
  int n;
  /*Variance de l'ecart: moyenne_ecart_carre-pow(moyenne_ecart,2);*/
  double variance_ecart=0;
  for(n=0;n<taille_mot_reference;n++){
    /* -1<= *(ecart_relatif+n) <=1 si il est initialise sinon il vaut -2*/
    /*       moyenne_ecart  est de meme --> differnece comrise entre -2 et 2 au carre c'est pareil*/
    variance_ecart+=pow((*(ecart_relatif+n) > -2 ? (*(ecart_relatif+n)-moyenne_ecart) : 2),2);
  }
  variance_ecart=variance_ecart/((double)taille_mot_reference);
  return variance_ecart;
}

double fctvariance_normalisee(double *ecart_relatif,int taille_mot_reference,double moyenne_ecart){  
  
  return(fctvariance_ecart(ecart_relatif,taille_mot_reference,moyenne_ecart)/((double)taille_mot_reference));
}


/*
 *les coefficient lettres_presentent,lettres_manquantent,lettres_en_trop, sont normalisées tout comme la variance*/
double calcul_du_fitness(double lettres_presentent,double lettres_manquantent,double lettres_en_trop,double variance_normalisee,double somme_ecart_relatif){
  return (coef_lettre_presente*lettres_presentent
	  +coef_lettre_manquante*lettres_manquantent
	  +coef_lettre_en_trop*lettres_en_trop
	  +variance_normalisee*somme_ecart_relatif*coef_variance);
}


/*
 *return fitness/fitness_du_mot_exact.
 *Cependant le fitness est calcule a partir de nbre de letres normalises
 *et des ecart_relatifs normalises:
 *cad
 *x/(taille_mot_dico*taille_mot_reference)
 *cad aussi que le fitness parfait vaut:
 * taille_mot_dico*coef_lettre_presente/(taille_mot_dico*taille_mot_reference)
 * cad :coef_lettre_presente/taille_mot_reference.
 */ 
double calcul_du_fitness_normalise(int taille_mot_dico,int taille_mot_reference,int lettres_presentent,int lettres_manquantent,int lettres_en_trop,double variance_ecart,double somme_ecart_relatif){
  double lettres_presententd=((double)lettres_presentent)/((double)(taille_mot_dico*taille_mot_reference));
  double lettres_manquantentd=((double)lettres_manquantent)/((double)(taille_mot_dico*taille_mot_reference));
  double lettres_en_tropd=((double)lettres_en_trop)/((double)(taille_mot_dico*taille_mot_reference));
  somme_ecart_relatif=somme_ecart_relatif/((double)(taille_mot_dico*taille_mot_reference));

  return (calcul_du_fitness(lettres_presententd,lettres_manquantentd,lettres_en_tropd,variance_ecart,somme_ecart_relatif)/((double)(coef_lettre_presente)/((double)taille_mot_reference)));
}



extern void pretraitement_fitness_liberation_de_l_espace(double *fitness_du_mot){
  free(fitness_du_mot);
}
