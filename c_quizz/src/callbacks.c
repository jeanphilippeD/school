/*
 *Definit l'enssemble des methodes appelles pour des actions sur des element graphiques(sauf pour la reponse)
 *Gere l'arrier plan graphique
 *
 *@author:DUFRAIGNE ZANON 
 */


#include <stdlib.h>
#include "callbacks.h"
#include <time.h>
#include "aquisition_question.h"
#include "traitementreponse.h"
#include "aquisition_dico.h"
#include "Ma_String_Entry.h"
#include "constante.h"
#include <libsx.h>
#include <math.h>

Widget Entete, Question, Reponse, Nbrpoints, Nbrpointstotal, Nomjoueur, Bquit, NiveauDifficulte, Difficile, Facile;
Widget bouton_validation_reponse, bouton_question_suivante,Fin,Aide, aide1, aide2,scrolniveau,niveau_jeu,temps_ecoule,nb_quest_faite;


void quit(Widget quit , void *d)
{
     exit(0);
}

int approchee(double d){
  int dessous=floor(d);
  if(d<dessous+0.5)
    return dessous;
  else
    return dessous+1;
}
/*
 *Score Final
 */
static int Score_Final=0;
/*
 */
char **questionetreponse;
int nombrequestion=10;
int nombre_mot_dico;
char **mot_du_dico;
  
static int nombre_de_point_total=0;
static int nombre_de_question_faites=0;
static int question_est_validee=1;
static int question_est_traitee=1;/*La question d'avant est finis d'etre traiter*/
static char facile[]="questionfacile.que";
static char difficile[]="questiondifficile.que";
char *fichierquestion=facile;
int choix_dificile=0;/*1 pour dificile*/

static int le_temp_ecoule=0;
static int le_niveau_jeu=0;/* indique le seul d'ajout des points*/
static int fin_jeu=0;

void formatte_message(Widget,char*);


void init_graine_random(void){
  time_t t;  /* declaration de la variable t*/
  fichierquestion=facile;
  questionetreponse=calloc(2,sizeof(char*));
  srand(time(&t));
  nombrequestion=aquisition_question(-1,fichierquestion,questionetreponse);
  aquisition_dico("Dico.dic",&mot_du_dico,&nombre_mot_dico);
    
}

void reinit(Widget b, void *q){
  nombre_de_point_total=0;
  nombre_de_question_faites=0;
  question_est_validee=1;
  question_est_traitee=1;
  le_temp_ecoule=0;
  le_niveau_jeu=0;
  fin_jeu=0;
  Score_Final=0;
  questsuiv(b,q);
}

void questsuiv(Widget bouton_question_suivante, void *q)
{
  
  if(question_est_traitee){
    if(!fin_jeu){
      nombre_de_question_faites++;
      /* génération de nombre aleatoire*/
      
      
      int r;
      /*srand(time(&t));*/			/* on met l'heure du PC dans t*/
      /*r=rand()%nombrequestion;*/		/* on genere un nombre aleatoire*/
      r=(int) (((double)nombrequestion)*rand()/(RAND_MAX+1.0));/* de 0 a nbre question-1*/
      
    
      
      aquisition_question(r,fichierquestion,questionetreponse);
      char *result=*(questionetreponse);
      formatte_message(Question,result);
      question_est_validee=0;
      autoriser_frappe();
      char *nbfaite=calloc(13+strlen("Nbre de questions "),sizeof(char));/*taille d'une chaine int*/
      sprintf(nbfaite,"Nbre de questions %04d",nombre_de_question_faites);
      SetLabel(nb_quest_faite,nbfaite);
      free(nbfaite);
    }
    else{
      reinit(bouton_question_suivante, q);
    }
  }

}

/*
 *message doit etre modifiable sans danger
 */
void formatte_message_sans_afficher(Widget w,char *messagee,char **retour){  
  int i;
  char *tmp;
  /*Calcul du nombre de ligne necessaires(et du nombre de \n)*/
  int taille_mess=strlen(messagee);/*pour l'affichage*/
  int nombre_de_caractere_inscriptible;/*pour une ligne*/
  XFont ma_font=GetFont(LA_FONT_UTILISEE);
  /* c'est la fonte du systeme unix et c'est bien*/
  if(ma_font==NULL){
    printf("font nulle\n");
    exit(1);
  }
  int taille_ref=TextWidth(ma_font,"MM");/*2 lettre majuscules et un espacecement entre 2 lettre */
  int taille_ref2=TextWidth(ma_font,"M");/*1 lettre majuscules*/
  /*si la lettre majuscule ne comprend pas l'espace entre deux lettre on compense*/
  nombre_de_caractere_inscriptible=((TAILLE_REPONSE-2*MARGE_LONGUEUR_POUR_REPONSE)/(taille_ref-taille_ref2));/*nombre de fois qu'une lettre rentre*/
  
  /*Combien de fois il faut cet espaces pour loger le message et allocation*/
  tmp=calloc(taille_mess+1+taille_mess/nombre_de_caractere_inscriptible+1,sizeof(char));
  i=0;
  while(taille_mess>nombre_de_caractere_inscriptible){
    memcpy(tmp+i+(i*nombre_de_caractere_inscriptible),messagee+(i*nombre_de_caractere_inscriptible),nombre_de_caractere_inscriptible);
    i++;
    *(tmp+i+(i*nombre_de_caractere_inscriptible)-1)='\n';
    taille_mess-=nombre_de_caractere_inscriptible;
  }
  /*taille_mess<=nombre_de_caractere_inscriptible*/
  memcpy(tmp+i+(i*nombre_de_caractere_inscriptible),messagee+(i*nombre_de_caractere_inscriptible),taille_mess+1);/*copie du '\0'*/
  
  *retour=tmp;
  
}

/*
 *Doit avoir ete alloue par un malloc calloc ...
 *ET AFFICHE
 */
void formatte_message(Widget w,char *messagee){
  char *tmp=NULL;
  formatte_message_sans_afficher(w,messagee,&tmp);
  SetTextWidgetText(Question,tmp,0);
  free(tmp);
}


void concatene_formate_et_affiche(Widget w,char *question,char *reponse_vrai){
  char *tmp;
  char *question_formatee=NULL;
  char *reponse_non_formatee;
  char *reponse_formatee=NULL;
  formatte_message_sans_afficher(w,question,&question_formatee);
  int taille_quest=strlen(question_formatee);
  int i,j;
  


  reponse_non_formatee=calloc(strlen(reponse_vrai)+1,sizeof(char));
  i=0;j=0;
  while(*(reponse_vrai+i)!='\0'){
    while(*(reponse_vrai+i)!=' '){
      *(reponse_non_formatee+j)=*(reponse_vrai+i);
      i++;j++;
    }
    i++;/*passage espace*/
    while(*(reponse_vrai+i)!=' ' && *(reponse_vrai+i)!='\0')
      i++;/*passage du chiffre*/
    /*recopie dernier caractere*/
    *(reponse_non_formatee+j)=*(reponse_vrai+i);
    j++;
    if(*(reponse_vrai+i)==' ')
      i++;/*passage espace sinon sortie*/
  }
  formatte_message_sans_afficher(w,reponse_non_formatee,&reponse_formatee);
  
  tmp=calloc(taille_quest+2+strlen(reponse_formatee)+1,sizeof(char));/*+2 :2'\n' remplace '\0'*/
  memcpy(tmp,question_formatee,taille_quest);
  *(tmp+taille_quest)='\n';
  *(tmp+taille_quest+1)='\n';
  memcpy(tmp+taille_quest+2,reponse_formatee,strlen(reponse_formatee)+1);
    
  SetTextWidgetText(Question,tmp,0);
  if(DEBUG){
    printf("formatee:\n%s\net reponse\n%s\net tout:\n%s",question_formatee,reponse_formatee,tmp);
  }

  free(reponse_non_formatee);
  free(question_formatee);
  free(reponse_formatee);
  free(tmp);
  
}
void validation_question(char *reponse){
  int nbrepoint;
  if(!question_est_validee){
    question_est_validee=1;
    question_est_traitee=0;
    /*copy pour travail*/
    int taille=strlen(reponse)+1;
    char *envoi=calloc(taille,sizeof(char));
    memcpy(envoi,reponse,taille);
    /**/
    
    /*AFFICHAGE VRAI REPONSE*/
    concatene_formate_et_affiche(Question,*(questionetreponse),*(questionetreponse+1));
    
    double les_pts_recus=traitementreponse((const int)nombre_mot_dico,(const char **)mot_du_dico,(const char *)*(questionetreponse+1),(const char *)reponse);
    
    /* respecte le niveau de jeu */
    if(le_niveau_jeu>(les_pts_recus*10.0)){
      nbrepoint=0;
    }
    else
      nbrepoint=approchee(les_pts_recus);
    
    nombre_de_point_total+=nbrepoint;
    if(choix_dificile)
      Score_Final+=nbrepoint*COTATION_DIFICULTE*((le_niveau_jeu+2)/DECOTATION_NIVEAU_DE_JEU);
    else
      Score_Final+=nbrepoint*((le_niveau_jeu+2)/DECOTATION_NIVEAU_DE_JEU);
    
        
    char *nbpoint=calloc(13+strlen("Nbre de Points "),sizeof(char));/*taille d'une chaine int*/
    char *nbpoint_total=calloc(13+strlen("Nbre de Points total "),sizeof(char));/*taille d'une chaine int*/
    char *nbfaite=calloc(13+strlen("Nbre de questions "),sizeof(char));/*taille d'une chaine int*/
    sprintf(nbpoint,"Nbre de Points %02d",nbrepoint);
    sprintf(nbpoint_total,"Nbre de Points total %04d",nombre_de_point_total);
    sprintf(nbfaite,"Nbre de questions %04d",nombre_de_question_faites);
    SetLabel(nb_quest_faite,nbfaite);
    SetLabel(Nbrpoints,nbpoint);
    SetLabel(Nbrpointstotal,nbpoint_total);
    free(nbfaite);
    free(nbpoint);/*la widget fait une copie*/
    free(nbpoint_total);
    
    question_est_traitee=1;/*Traitement finis*/
    
  }


}

void fermer_fenetre(Widget w,void *d){
  CloseWindow();
}

void aide(Widget Aide, void *ai)
{
  
  MakeWindow("Aide au bon fonctionnement du Quiz", SAME_DISPLAY, 0);
  {
    
    
    Entete = MakeLabel ("Aide au bon fonctionnement du Quiz ");
    SetWidgetPos(Entete, NO_CARE, NULL, NO_CARE, NULL);
    Widget aide1=MakeLabel ("Pour pouvoir entrer votre réponse, veuillez laisser le curseur de la souris dans la fenêtre active");
    SetWidgetPos(aide1, NO_CARE, NULL, PLACE_UNDER, Entete);
    Widget aide2=MakeLabel ("Veuillez entrer tous les mots de votre réponse en minuscule sauf pour les noms propre");
    SetWidgetPos(aide2, NO_CARE, NULL, PLACE_UNDER, aide1);
    Bquit = MakeButton("Quit",fermer_fenetre, NULL);
    SetWidgetPos(Bquit, PLACE_RIGHT, Entete, NO_CARE, NULL);
    ShowDisplay();
    SetColor(GetRGBColor(0,170,255));
  }
  
}

void questfac(Widget NiveauDifficulte , void *f)
{
  if(question_est_traitee){
    choix_dificile=0;
    fichierquestion=facile;
    nombrequestion=aquisition_question(-1,fichierquestion,questionetreponse);
    questsuiv(NiveauDifficulte,f);
  }
  else
    Beep();
}

void questdif(Widget NiveauDifficulte , void *f)
{
  if(question_est_traitee){
    choix_dificile=1;
    fichierquestion=difficile;
    nombrequestion=aquisition_question(-1,fichierquestion,questionetreponse);
    questsuiv(NiveauDifficulte,f);
  }
  else
    Beep();
}

void scroll_func(Widget w, float new_val, void *data)
{
  le_niveau_jeu=approchee(new_val);
  
  if(DEBUG)
    printf("scrol %f\n",new_val);
  
  char *nivjeu=calloc(13+strlen("Rejet a: %%"),sizeof(char));/*taille d'une chaine int*/
  sprintf(nivjeu,"Rejet a: %02d%%",le_niveau_jeu);
  SetLabel(niveau_jeu,nivjeu);
}


void  Pour_temp(void *data, XtIntervalId *id)
{
  if(!question_est_validee)
    le_temp_ecoule++;
  

  char *temps=calloc(13+strlen("Temps: "),sizeof(char));/*taille d'une chaine int*/
  sprintf(temps,"Temps: %04d",le_temp_ecoule);
  SetLabel(temps_ecoule,temps);
  free(temps);
  AddTimeOut(1000,(void*)Pour_temp,NULL);
  

}


void fin_questions(Widget w, void *data){
  if(question_est_traitee){
    
    /*
     *empeche de repondre
     */
    fin_jeu=1;
    devalider_clavier();
    question_est_validee=1;
    /*
     */
    char *tmp=NULL;
    int tailletmp;
    int score_total_calcule=(10000*Score_Final)/((nombre_de_question_faites+AUGMENTATION_NOMBRE_QUESTION)*(le_temp_ecoule+AUGMENTATION_TEMPS));
    
    formatte_message_sans_afficher(w,"Votre score calculee a partir de tous les        parametre est: ",&tmp);
    tailletmp=strlen(tmp);
    char *affiche=calloc(13+tailletmp,sizeof(char)+2);/*taille d'une chaine int +\n\n*/
    memcpy(affiche,tmp,tailletmp);/*sans \0*/
    *(affiche+tailletmp)='\n';
    *(affiche+tailletmp+1)='\n';
    sprintf((affiche+tailletmp+2),"%d",score_total_calcule);
    SetTextWidgetText(Question,affiche,0);
    
    free(tmp);
    free(affiche);
  }
  else
    Beep();
  
}
