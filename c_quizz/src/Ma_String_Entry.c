/*
 *Definit une implementation de String Entry
 *Permet d'utiliser les accents ciconflexe et tremas
 *Permet de reagir activement a l'action de l'utilisateur
 *
 *@author:DUFRAIGNE ZANON 
 */
#include <stdlib.h>
#include "callbacks.h"
#include "Ma_String_Entry.h"
#include "constante.h"


Pour_data_reponse *le_data=NULL;

/*
 *TOUT POUR LA RECEPTION DE LA REPONSE
 */

/*
 *apelle normalement qu'une seule fois
 */
Widget Mon_entree(void){
  
  /*
   *Reglage de la font et des tailles
   */                    
  XFont ma_font=GetFont(LA_FONT_UTILISEE);
  /* c'est la fonte du systeme unix et c'est bien*/
  if(ma_font==NULL){
    printf("font nulle\n");
    return NULL;
  }
  else
    if(DEBUG)
    printf("font non nulle\n");
  /* On a notre font */
  /* Reglage hauteur de la zone */
  int taille_font=FontHeight(ma_font);
  int hauteur_zone=taille_font+2*MARGE_HAUTEUR_POUR_REPONSE;
  /*FIN*/
  int nombre_de_caractere_inscriptible;
  int taille_ref=TextWidth(ma_font,"MM");/*2 lettre majuscules et un espacecement entre 2 lettre */
  int taille_ref2=TextWidth(ma_font,"M");/*1 lettre majuscules*/
  /*si la lettre majuscule ne comprend pas l'espace entre deux lettre on compense*/
  nombre_de_caractere_inscriptible=((TAILLE_REPONSE-2*MARGE_LONGUEUR_POUR_REPONSE)/(taille_ref-taille_ref2));/*nombre de fois qu'une lettre rentre*/
  
  
  /*
   *Initialisation de le_data
   */
  char tmp[]=" Veuillez saisir votre reponse";
  int taille=strlen(" Veuillez saisir votre reponse");
  char *message=calloc(taille+1,sizeof(char));/*Contient le message a afficher */
  /* calloc pour utiliser realloc*/
  memcpy(message,tmp,taille*sizeof(char));
  *(message+taille)='\0';/*permet de bien se rendre compte qu'il sagit d'une chaine de caractere(normalement equivalent a memcpy(message,tmp,taille+1);)*/
  /* mesage est BON*/
  
  
  le_data=(Pour_data_reponse*)calloc(1,sizeof(Pour_data_reponse));
  if(le_data==NULL)return NULL;/* Ya pas la place*/
  
  le_data->position_premier_lettre = 0;
  le_data->position_curseur = 0;
  le_data->message =message;
  le_data->taille_message=taille;
  le_data->nombre_de_lettre_affichable=nombre_de_caractere_inscriptible;
  /*
   *FinInitialisation le data
   */
  
  /*C'est bon on peut creer la widget*/  
  Widget retour=MakeDrawArea(TAILLE_REPONSE,hauteur_zone,redisplay_la_reponse,le_data);
  
  SetWidgetFont(retour,ma_font);
  SetKeypressCB(retour,frappe_de_la_reponse);
  
  return retour;
}







static int clavier_valide=0; /*Valide a la question suivante*/
static int reinitialisation_premiere_touche=0;/*Permet defacer le message conenu lors de la frappe de la premiere touche active*/
extern void autoriser_frappe(void){
  reinitialisation_premiere_touche=0;
  clavier_valide=1;
}
void devalider_clavier(void){
  clavier_valide=0;/*devalide le clavier revalider a la prochaine question*/
}

void reponse_validee(void *data){
  
  
  validation_question(((Pour_data_reponse*)data)->message);
}

void reinit_data(Widget w,void *data){
  
  Pour_data_reponse *datar=(Pour_data_reponse*)data;
  
  
  XFont ma_font=GetWidgetFont(w);
  
  int nombre_de_caractere_inscriptible;
  int taille_ref=TextWidth(ma_font,"MM");/*2 lettre majuscules et un espacecement entre 2 lettre */
  int taille_ref2=TextWidth(ma_font,"M");/*1 lettre majuscules*/
  /*si la lettre majuscule ne comprend pas l'espace entre deux lettre on compense*/
  nombre_de_caractere_inscriptible=((TAILLE_REPONSE-2*MARGE_LONGUEUR_POUR_REPONSE)/(taille_ref-taille_ref2));/*nombre de fois qu'une lettre rentre*/
  
  free(datar->message);
  char *nvllereponse=(char*)calloc(1+1,sizeof(char));/*1 pour le'\0'*/
  *nvllereponse=' ';
  *(nvllereponse+1)='\0';
  
  datar->message =nvllereponse;
    
  datar->position_premier_lettre = 0;
  datar->position_curseur = 1;
  datar->taille_message=1;
  datar->nombre_de_lettre_affichable=nombre_de_caractere_inscriptible;
}



/*
 *width et heigh ne seront pas vraiment untilises car data contient toute les donnes utiles qui on bien entendue ete traites en fonction de la hauteur et de la largeur
 */
void redisplay_la_reponse(Widget w, int width, int height, void *data)
{
  char curseur;
  static int curseur_visible=0;/* une fois visible une fois pas 1 oui 0 non*/
  static int switcoupas=1;
  /*switch du curseur a chaque appui et relachement(ou redimmensionnement ce qui rend le curseur inverse de ce qu'il etait*/
  if(switcoupas){
    if(curseur_visible)
      curseur_visible=0;
    else
      curseur_visible=1;
    /**/
    switcoupas=0;
  }
  else
    switcoupas=1;
  
  /*evite la surcharge en ecriture*/
  /*pourra etre remplacer dans une optimisation ulterieure*/
  Pour_data_reponse *datar=(Pour_data_reponse*)data;
  char *reponse=datar->message;
  int position_premier_lettre=datar->position_premier_lettre;
  int position_curseur=datar->position_curseur;
  int taille_message=datar->taille_message;
  int nombre_de_lettre_affichable=datar->nombre_de_lettre_affichable;
  /**/
  
  char *affichage_reel;/*chaine reelement affichee*/


  int position_curseur_relative;
  position_curseur_relative=position_curseur-position_premier_lettre;
  int taille_de_la_chaine_a_afficher=taille_message-position_premier_lettre;

  /*Si le reste du message est trop long on va le tronquer*/
  if(taille_de_la_chaine_a_afficher>nombre_de_lettre_affichable)
    taille_de_la_chaine_a_afficher=nombre_de_lettre_affichable;
  
  affichage_reel=(char*)calloc(taille_de_la_chaine_a_afficher+2,sizeof(char));/*dans tous les cas(perte d'au plus un octet de memoire(1pour le curseur 1 pour le '\0'*/
  
  
  if(position_curseur==taille_message){/*on doit lui laisser la place*/
    if(taille_message==0){
      if(curseur_visible)
	curseur='_';
      else
	curseur=' ';
      
      *(affichage_reel)=curseur;/*juste avant le'\0'*/
      *(affichage_reel+1)='\0';
    }
    else{
      if(taille_de_la_chaine_a_afficher==nombre_de_lettre_affichable){/*les car prenne toute la place*/
	taille_de_la_chaine_a_afficher=nombre_de_lettre_affichable-1;
	
      }
      /*sinon c'est bon*/
      *(affichage_reel+taille_de_la_chaine_a_afficher+1)='\0';/*d'ou le +2*/
      
      memcpy(affichage_reel,reponse+position_premier_lettre,taille_de_la_chaine_a_afficher*sizeof(char));
      /*curseur*/
      if(curseur_visible)
	curseur='_';
      else
	curseur=' ';
      
      *(affichage_reel+taille_de_la_chaine_a_afficher)=curseur;/*juste avant le'\0'*/
      /**/
    }
  }
  else{/*on peut prendre la place*/
    *(affichage_reel+taille_de_la_chaine_a_afficher)='\0';/*dans tous les cas*/
    memcpy(affichage_reel,reponse+position_premier_lettre,taille_de_la_chaine_a_afficher*sizeof(char));
    
    /*curseur*/
    /*soit le curseur soit la lettre*/
    if(curseur_visible)
      curseur='_';
    else
      curseur=*(affichage_reel+position_curseur_relative);
    
    *(affichage_reel+position_curseur_relative)=curseur;
    /**/
    
  }


  
  /*affichage du texte avec les marges*/
  /*le fond est repeint avec la couleur du fond*/
  GetStandardColors();
  
  SetBgColor(w,WHITE);
  
  ClearDrawArea();
  
  SetFgColor(w,BLACK);
  DrawText(affichage_reel,MARGE_LONGUEUR_POUR_REPONSE,FontHeight(GetWidgetFont(w))+MARGE_HAUTEUR_POUR_REPONSE);
  
  free(affichage_reel);
  
  
}

static int modificateur_d_acent=0;
void ajout_caractere(int c,void *data){
  /*printf("%c %d\n",i,i);*/
  modificateur_d_acent=0;

  /*evite la surcharge en ecriture*/
  /*pourra etre remplacer dans une optimisation ulterieure*/
  Pour_data_reponse *datar=(Pour_data_reponse*)data;
  char *reponse=datar->message;
  int position_premier_lettre=datar->position_premier_lettre;
  int position_curseur=datar->position_curseur;
  int taille_message=datar->taille_message;
  int nombre_de_lettre_affichable=datar->nombre_de_lettre_affichable;
  /**/
  
  
  taille_message++;
  char *nvllereponse=(char*)calloc(taille_message+1,sizeof(char));/*pour le'\0'*/
  memcpy(nvllereponse,reponse,position_curseur);/*copie avant la car pos_cur-1*/
  *(nvllereponse+position_curseur)=(char)c;/*faut pas lui mettre un entier*/
  memcpy((nvllereponse+position_curseur+1),(reponse+position_curseur),(taille_message-1+1-position_curseur));/*+1 pour le '\0''  -1 car le reste est de la taille d'avant - position curseur    : copier sur et apres*/
  
  
  /*Mise a jour des data*/
  (datar->position_curseur)++;
  (datar->taille_message)=taille_message;
  
  /**/
  free(datar->message);
  datar->message = nvllereponse;
  /**/
  if(position_curseur-position_premier_lettre>nombre_de_lettre_affichable-1){
    (datar->position_premier_lettre)++;/*decalage car on ne peut plus afficher le curseur sinon*/
  }
  /*printf("taile mes:%d pos_prem:%d pos_cur:%d nb-let_aff:%d\n%s\n",datar->taille_message,datar->position_premier_lettre,datar->position_curseur,datar->nombre_de_lettre_affichable,datar->message);*/
  /*data est mis a jour*/

}

void  frappe_de_la_reponse(Widget w, char *input, int up_or_down, void *data)
{
  
  if(clavier_valide){
    if(reinitialisation_premiere_touche==0){/* reinitialisation du data*/
      reinit_data(w,data);
      reinitialisation_premiere_touche=1;
    
    }
    /*evite la surcharge en ecriture*/
    /*pourra etre remplacer dans une optimisation ulterieure*/
    /*FAIRE TRES ATTENTION AU VARIABLE QUE L'ON MODIFIE*/
    Pour_data_reponse *datar=(Pour_data_reponse*)data;
    char *reponse=datar->message;
    int position_premier_lettre=datar->position_premier_lettre;
    int position_curseur=datar->position_curseur;
    int taille_message=datar->taille_message;
    int nombre_de_lettre_affichable=datar->nombre_de_lettre_affichable;
    /**/
    
    if(!up_or_down){
      if(strlen(input)>1){/*ce n'est pas un vrai caraxctere*/
	/* les modificateur de majuscule et de alt ne seront pas pris en compte seul les lettres modifies seront capte*/
	/*On va capturer les fleches gauche et droites*/
	if(!strcmp("Right",input)){
	  /*Mise a jour des data*/
	  if(position_curseur<taille_message){
	    position_curseur++;
	    (datar->position_curseur)++;
	    
	    if(position_curseur-position_premier_lettre>nombre_de_lettre_affichable-1){
	      (datar->position_premier_lettre)++;/*decalage car on ne peut plus afficher le curseur sinon*/
	    }
	  }
	  
	}
	if(!strcmp("Left",input)){
	  /*Mise a jour des data*/
	  if(position_curseur>1){
	    position_curseur--;
	    (datar->position_curseur)--;
	    
	    if(position_curseur<=position_premier_lettre){
	      (datar->position_premier_lettre)--;/*decalage car on ne peut plus afficher le curseur sinon*/
	    }
	    
	  }
	  
	}
	
      }
      else{/*c'est un caractere que l'on doit ecrire*/
	int caractere=*input;
	switch(caractere){
	case 8:/*DEL*/
	  if(position_curseur>1){/*on efface avant 1 car avant il n'y a pas de caractere*/
	    taille_message--;
	    char *nvllereponse=(char*)calloc(taille_message+1,sizeof(char));/*pour le'\0'*/
	    memcpy(nvllereponse,reponse,position_curseur-1);/*copie avant la car pos_cur-1*/
	    memcpy((nvllereponse+position_curseur-1),(reponse+position_curseur),(taille_message+1+1-position_curseur));/*+1 pour le '\0''  copier sur et apres*/
	    
	    /*Mise a jour des data*/
	    position_curseur--;
	    (datar->position_curseur)--;
	    (datar->taille_message)--;
	    
	    /**/
	    free(datar->message);
	    datar->message = nvllereponse;
	    /**/
	    if(datar->position_curseur<(datar->position_premier_lettre)){
	      (datar->position_premier_lettre)--;/*decalage car on ne peut plus afficher le curseur sinon*/
	    }
	    /*data est mis a jour*/
	    /*c'est bon*/
	  }break;
	case 127:/*SUPPR*/
	  if(position_curseur<taille_message){/*on efface sur*/
	    char *nvllereponse=(char*)calloc(taille_message-1+1,sizeof(char));/*pour le'\0'*/
	    memcpy(nvllereponse,reponse,position_curseur);/*copier avant*/
	    memcpy((nvllereponse+position_curseur),(reponse+position_curseur+1),(taille_message+1-1-position_curseur));/*+1 pour le '\0'  -1 car on a suprimer un caractere appres :copier  apres*/
	  
	    /*Mise a jour des data*/
	    /*(datar->position_curseur) ne change pas*/
	    (datar->taille_message)--;
	    
	    /**/
	    free(datar->message);
	    datar->message = nvllereponse;
	    /**/
	    /*Si la taile du message vaut 0 alors la premiere lettre est aussi a 0 D'OU EXCEPTION*/
	  /*(datar->position_premier_lettre)ne change pas*/
	    
	  /*data est mis a jour*/
	  /*c'est bon*/
	  }break;
	case 13:/*entree*/
	  devalider_clavier();
	  reponse_validee(data);
	  break;
	  
	  
	case 96:/*Modificateur '`'->è */
	  modificateur_d_acent=1;
	  break;
	case 39:/*Modificateur '''->é car pas d'apostrophe*/
	modificateur_d_acent=2;
	break;
	case 94:/*Modificateur '^'*/
	  modificateur_d_acent=3;
	  break;
	case -88:/*Modificateur '"'->ë */
	  modificateur_d_acent=4;
	  break;
	
	  
	case 9:/*TAB*/break;/*rien*/
	case -78:/*carre*/break;/*rien*/
	  
	case 97:/* a */
	  switch(modificateur_d_acent){
	  case 1:/*Modificateur '`'->à */
	    ajout_caractere('à',data);
	  break;
	  case 3:/*Modificateur '^'->â */
	    ajout_caractere('â',data);
	    break;
	  case 4:/*Modificateur '"'->ë */
	    ajout_caractere('ä',data);
	    break;
	  default: ajout_caractere(caractere,data);break;
	  }break;
	
	case 101:/* e */
	  switch(modificateur_d_acent){
	  case 1:/*Modificateur '`'->è */
	    ajout_caractere('è',data);
	    break;
	  case 2:/*Modificateur '''->é */
	    ajout_caractere('é',data);
	    break;
	  case 3:/*Modificateur '^'->ê */
	    ajout_caractere('ê',data);
	    break;
	  case 4:/*Modificateur '"'->ë */
	    ajout_caractere('ë',data);
	    break;
	  default: ajout_caractere(caractere,data);break;
	  }break;
	  
	case 105:/* i */
	  switch(modificateur_d_acent){
	  case 3:/*Modificateur '^'->â */
	    ajout_caractere('î',data);
	    break;
	  case 4:/*Modificateur '"'->ë */
	    ajout_caractere('ï',data);
	    break;
	  default: ajout_caractere(caractere,data);break;
	  }break;
	  
	case 111:/* o */
	switch(modificateur_d_acent){
	case 3:/*Modificateur '^'->â */
	  ajout_caractere('ô',data);
	  break;
	case 4:/*Modificateur '"'->ë */
	  ajout_caractere('ö',data);
	  break;
	  default: ajout_caractere(caractere,data);break;
	}break;
	
	case 117:/* u */
	  switch(modificateur_d_acent){
	  case 1:/*Modificateur '`'->à */
	    ajout_caractere('ù',data);
	    break;
	  case 3:/*Modificateur '^'->â */
	    ajout_caractere('û',data);
	    break;
	  case 4:/*Modificateur '"'->ë */
	    ajout_caractere('ü',data);
	    break;
	  default: ajout_caractere(caractere,data);break;
	  }break;
	  /* En Francais les majuscules ne sont pas accentuees*/
	default:
	ajout_caractere(caractere,data);
	break;
	
	}
      }
      
    }
    redisplay_la_reponse(w,TAILLE_REPONSE+MARGE_LONGUEUR_POUR_REPONSE*2,FontHeight(GetWidgetFont(w))+MARGE_HAUTEUR_POUR_REPONSE,data);
  }
  else
    if(!up_or_down)
      Beep();
}
