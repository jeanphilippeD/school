/*
 *
 *Definit l'enssemble des constantes necessaires au programe ce qui facilite leur modification notament DEBUG
 *
 *@author:DUFRAIGNE ZANON 
 */

/*
 *main.c
 */
/* Définition taille fenetre */
#define X_SIZE 400
#define Y_SIZE 400
#define HAUTEUR_QUESTION 100
/*Pour la reponse*/


/*
 *Ma_String_Entry.c
 *TOUT POUR LA RECEPTION DE LA REPONSE
 */

#define TAILLE_REPONSE 350/*Utilisee aussi pour la largeur question*/
#define MARGE_HAUTEUR_POUR_REPONSE 5 /* en pixels en haut et en bas (toale= marge*2)*/
#define MARGE_LONGUEUR_POUR_REPONSE 5 /* en pixels en avant et apres (toale= marge*2)*/
#define LA_FONT_UTILISEE "-adobe-courier-bold-o-normal--12-120-75-75-m-70-iso8859-1"
/* font utilise pour l'entre de la reponse*/
/*
 */


/*
 *Pretraitement_fitness.c
 */
#define coef_lettre_presente 1000
#define coef_lettre_manquante -500
#define coef_lettre_en_trop -500
#define coef_variance -1000

#define taille_table_de_caracteres 256 /*taille de la table utilise ici iso_8859-1(UNIX)*/

#define type_of_caractere 128 /* 0 pour non signe 128 pour signe car va de -128 a 127*/

/*
 */



/*
 *pretraitement_rang.c
 */
#define epsilon 1E-12 /*Neglige diference de rang sous ce seuil*/
#define Nombre_Max_de_Rang_Identiques 100
/*impose une limite au rang pour empecher l'utilisation de mot inexistant pour gagner des points:
 *Si le mot est tres eloigne de tout(mais sans plus),les fitness sont proche et on se retrouve avec beaucoup de rang proche de 0 ce qui n'est pas bon.
 *C'est bien une limite arbitraire qui depend en fait du dico utilise
 */
					   
/*
 */

/*
 *pretraitement_un_mot.c
 */
/*evite les problemes dues aux pertes de precisions*/
#define epsilon2 1E-14/* pour que le rang soit sur d'etre bon*/
/*
 */

/*
 *callbacks.c
 */
#define COTATION_DIFICULTE 5 /* indique l'effet sur le score de la difficulte*/
#define DECOTATION_NIVEAU_DE_JEU 2
#define AUGMENTATION_TEMPS 10 /* a partir de 100 seconde l'effe sra moins perceptible*/
#define AUGMENTATION_NOMBRE_QUESTION 5 /* a partir de 50 question l'effe sra moins perceptible*/



/*
 *Mode deboguage
 */
#define DEBUG 0
/*
 */

