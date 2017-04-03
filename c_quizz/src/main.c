/*
 *Point d'entre du programme
 *Definit l'interface graphique et lance le jeu
 *
 *@author:DUFRAIGNE ZANON 
 */
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "callbacks.h"
#include "Ma_String_Entry.h"
#include "aquisition_question.h"
#include "constante.h"



void init_display(int,char**,void*);
void menudifficulte(void);



int main(int argc, char **argv){


  if (OpenDisplay(argc, argv) == 0){
     fprintf(stderr,"Can't open display\n");
     return 1;
   }


  init_graine_random();

  menudifficulte();

  init_display(argc, argv, NULL);  /* setup the display */

    
  MainLoop();                      /* lancement du fonctionnent interactif de l'application */
  
  return 0;
}




void init_display(int argc, char **argv,void *d)
{

  
  
  Reponse=Mon_entree();
  /*avtca*/
  /* création et assemblage des widgets */

  Entete = MakeLabel ("QUIZ                                       ");

  /*
   */
  Question = MakeTextWidget("Bonjour, pour commencer\nCliquez sur Question suivante\naprès avoir choisis votre niveau", 0, 0, TAILLE_REPONSE,HAUTEUR_QUESTION);/*Zone graphique contenant la question */
  SetWidgetFont(Question,GetFont(LA_FONT_UTILISEE));
  /*
   */

  
  /*
   */
  nb_quest_faite=MakeLabel ("Nbre de questions 0000");
  Nbrpoints = MakeLabel ("Nbre de Points 00");      		/*Zone graphique contenant le nombre de points de laquestion répondue */
  Nbrpointstotal = MakeLabel ("Nbre de Points total 0000");           /*Zone graphique contenant le nombre total de points gagné */
  Nomjoueur  = MakeLabel ("0");                			/*Zone graphique contenant le nom du joueur */
  /*
   */

  /*création des boutons*/
  
  Bquit = MakeButton("  Quit  ", quit, NULL);
  Widget Aide = MakeButton("  Aide  ", aide , NULL);
  
  /* Aqcuisition question */
  bouton_question_suivante=MakeButton (" Question Suivante ", questsuiv, NULL);
  Fin=MakeButton ("   FIN QUESTIONS   ", fin_questions, NULL);
  /*Difficulte = MakeButton("Niveau Difficulté", NULL, NULL);*/
  /*Difficile = MakeButton("Difficile",NULL, NULL);*/
  /*Facile = MakeButton("Facile", NULL, NULL);*/
  
  
  /*
   */
  scrolniveau=MakeVertScrollbar(2,scroll_func,NULL);
  SetScrollbar(scrolniveau,0.0,99.0,99.0);
  niveau_jeu=MakeLabel("Rejet a: 00%%");
  
  /*
   */
  /*Temps
   */
  temps_ecoule=MakeLabel ("Temps: 0000");
  AddTimeOut(1000,(void*)Pour_temp,NULL);
  /*
   */

  /*
   *Positionnement de ces Widget
   */
  
  SetWidgetPos(Entete, NO_CARE, NULL, NO_CARE, NULL);
  
  SetWidgetPos(Question, NO_CARE, NULL,PLACE_UNDER, Entete );
  SetWidgetPos(/*MakeStringEntry("Veuillez saisir votre reponse", 250, entrereponse, NULL)*/Reponse, NO_CARE, NULL, PLACE_UNDER,Question);
  
  SetWidgetPos(bouton_question_suivante, PLACE_RIGHT, Question, NO_CARE, NULL);
  SetWidgetPos(Fin, PLACE_RIGHT, Question,PLACE_UNDER, bouton_question_suivante);
  SetWidgetPos(Bquit, PLACE_UNDER, Fin,PLACE_RIGHT, Question);
  SetWidgetPos(Aide,PLACE_UNDER, Fin, PLACE_RIGHT, Bquit);
  
  
  SetWidgetPos(NiveauDifficulte,PLACE_UNDER,Bquit,PLACE_RIGHT, Question);
  SetWidgetPos(scrolniveau,PLACE_UNDER,NiveauDifficulte,PLACE_RIGHT, Question);
  SetWidgetPos(niveau_jeu,PLACE_UNDER,NiveauDifficulte,PLACE_RIGHT,scrolniveau);


  SetWidgetPos(nb_quest_faite,PLACE_RIGHT,bouton_question_suivante,PLACE_UNDER,scrolniveau);
  SetWidgetPos(Nbrpoints, PLACE_RIGHT,bouton_question_suivante, PLACE_UNDER,nb_quest_faite);
  /*SetWidgetPos(bouton_validation_reponse, NO_CARE, NULL, PLACE_UNDER, Nbrpoints);*/
  
  SetWidgetPos(Nbrpointstotal, PLACE_RIGHT, bouton_question_suivante, PLACE_UNDER, Nbrpoints);
  

  SetWidgetPos(temps_ecoule,PLACE_RIGHT,bouton_question_suivante, PLACE_UNDER,NiveauDifficulte);
  
  ShowDisplay();  	/* pour afficher l'interface */

  GetStandardColors();  /* pour gerer les couleurs */
  
  SetBgColor(Bquit,GetRGBColor(94,48,244));
  SetBgColor(Aide,GetRGBColor(216,43,43));
  SetBgColor(Entete,GetRGBColor(136,206,113));
  SetBgColor(bouton_question_suivante,GetRGBColor(208,13,219));
  SetBgColor(Question,GetRGBColor(244,247,168));
  SetBgColor(Nbrpoints,GetRGBColor(54,218,226));
  SetBgColor(Nbrpointstotal,GetRGBColor(0,170,255));
  SetBgColor(NiveauDifficulte,GetRGBColor(211,125,63));
  SetColor(GetRGBColor(200,0,50));
}

void menudifficulte(void){
  
  
   NiveauDifficulte = MakeMenu("Niveau de Difficulte");
   
   MakeMenuItem(NiveauDifficulte,"Facile", questfac, NULL );
   MakeMenuItem(NiveauDifficulte, "Difficile", questdif, NULL );
   
}

