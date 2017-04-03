/*Include des librairies*/
#include <stdio.h>
#include <libsx.h>

/*Termine l'application*/
extern void quit(Widget, void *);



extern Widget Entete, Question, Reponse, Nbrpoints, Nbrpointstotal, Nomjoueur, Bquit, NiveauDifficulte, Difficile, Facile;
extern Widget bouton_validation_reponse, bouton_question_suivante,Fin,Aide, aide1, aide2,scrolniveau,niveau_jeu,temps_ecoule,nb_quest_faite;

extern void init_graine_random(void);

extern void validation_question(char*);

extern void questsuiv(Widget,void*);

extern void aide(Widget, void* );

extern void questfac(Widget , void *);

extern void questdif(Widget , void *);

extern void scroll_func(Widget,float,void*);

extern void  Pour_temp(void*,XtIntervalId*);

extern void fin_questions(Widget, void*);
