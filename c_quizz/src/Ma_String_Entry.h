 


typedef struct Pour_data_reponse
{
  /*lettre_affichee :nombre de lettre a afficher reelement
    =min(taille_message-position_premier_lettre,nombre_de_lettre_affichable)-1
    -1 uniquement si le curseur est en fin de chaine pour l'inscription d'un nvx caractere
  */
  int taille_message;/*sans compter le caracter de fin non affichable*/
  int nombre_de_lettre_affichable;/*initialise au debut, depend de la font et de la taille de la zone*/
  int position_premier_lettre;/*position de la premier letre a afficher
				doit etre inferieure au nombre de lettre du message
			      */
  int position_curseur;/*comme son nom l'indique
			*doit etre ds les position affichable
			*/
  char *message;
  
}Pour_data_reponse;

extern Widget Mon_entree(void);


extern void  frappe_de_la_reponse(Widget,char*,int,void*);

extern void  redisplay_la_reponse(Widget,int,int,void*);

extern void autoriser_frappe(void);

extern void devalider_clavier(void);
