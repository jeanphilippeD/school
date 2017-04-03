/*
 *Permet de trier les tableau couple fitness-mot par ordre decroissant
 *
 *2 Methodes : Utilisation de la plus rapide
 *
 *@author:DUFRAIGNE ZANON 
 */



void echanger(char**,double*,int,int);
void tri_par_selection_directe(int,char**,double*);
void tri_rapide_total(int,char**,double*);
/*
 *tri par fitness croissant
 *tri par selection directe
 *tri rapide
 *2 methode 1simple une complexe et plus rapide
 *utilisation du tri rapide est  justifier 
 */
extern void tri_par_fitness_croissant(int nbre_mot,char **mots_du_dico,double *fitness_du_mot){
  /*tri_par_selection_directe(nbre_mot,mots_du_dico,fitness_du_mot);*/
  tri_rapide_total(nbre_mot,mots_du_dico,fitness_du_mot);
}

  
void tri_par_selection_directe(int nbre_mot,char **mots_du_dico,double *fitness_du_mot){
  int n;
  int m;
  int rang_du_max;
  double fitness_du_max;
  for(n=0;n<nbre_mot;n++){/*parcourt et tri de toute la liste*/
    /*antecedant:la sous-liste de rang < a n est trie*/
    /*conséquent:la sous-liste de rang < a n+1 est triee*/
    
    rang_du_max=n;
    fitness_du_max=*(fitness_du_mot+n);/*variable supressible mais qui facilite la lecture*/
    for(m=n+1;m<nbre_mot;m++){/*parcourt et tri de toute la sous-liste non trie restante pour recherche du maximum(exepte le 1er deja pris)*/
      if(fitness_du_max<*(fitness_du_mot+m)){
	rang_du_max=m;
	fitness_du_max=*(fitness_du_mot+m);
      }
      /*sinon rien*/
    }
    /*rang_du_max contient le rang du max*/
    echanger(mots_du_dico,fitness_du_mot,n,rang_du_max);
  }
  /*la liste est triee*/
}


/*
 *Sous liste entre gauche et droit
 */
void tri_rapide(char **mots_du_dico,double *fitness_du_mot,int gauche,int droit){
  int i=gauche;
  int j=droit;
  double pivot=*(fitness_du_mot+(i+j)/2);
  do{
    while(*(fitness_du_mot+i)>pivot)i++;
    while(*(fitness_du_mot+j)<pivot)j--;
    if(i<=j){
       echanger(mots_du_dico,fitness_du_mot,i,j);
       i++;
       j--;
    }
    /*Invariant
     * ququesoit k appartenant a [1;i-1] *(fitness+k)<=pivot
     * ququesoit k appartenant a [j+1;longueurliste] *(fitness+k)>=pivot
     */
  }
  while(i<=j);
  /* ququesoit k appartenant a [j+1;i-1] *(fitness+k)==pivot
   * ququesoit k appartenant a [1;i-1] *(fitness+k)<=pivot
   * ququesoit k appartenant a [j+1;longueurliste] *(fitness+k)>=pivot
   */
  if(gauche<j)
    tri_rapide(mots_du_dico,fitness_du_mot,gauche,j);
  if(droit>i)
    tri_rapide(mots_du_dico,fitness_du_mot,i,droit);
}

/*
 *declenche le tri rapide
 */
void tri_rapide_total(int nbre_mot,char **mots_du_dico,double *fitness_du_mot){
  tri_rapide(mots_du_dico,fitness_du_mot,0,nbre_mot-1);
}

/*
 *rang numerotes de 0 a nbre_mot-1.
 *echange 2 elements de rang rang1 et rang2 en gardant les corespondance mot-fitness.
 */
void echanger(char **mots_du_dico,double *fitness_du_mot,int rang1,int rang2){
  char *mot_tmp=*(mots_du_dico+rang1);/*le mieux est d'echanger les pointeurs sans changer phisiquement le mot de place*/
  double fit_tmp=*(fitness_du_mot+rang1);
  
  /*affectation des novelles valeurs*/
  *(mots_du_dico+rang1)=*(mots_du_dico+rang2);
  *(fitness_du_mot+rang1)=*(fitness_du_mot+rang2);
  
  *(mots_du_dico+rang2)=mot_tmp;
  *(fitness_du_mot+rang2)=fit_tmp;
  /*FIN*/
}
