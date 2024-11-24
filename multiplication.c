#include "csc.h"
#include "vecteurCreux.h"
#include "multiplication.h"

vecCreux *multiplication_CSCtrgl_vec_creux(CSC *ptrCSC, vecCreux *vec) 
{
    //compter les non zero du nouveau vecteur
    //creer le nouveau vecteur avec le bon nombre de index
    //remplir le vecteur avec les valeurs calculees
    vecCreux *new = (vecCreux *)malloc(sizeof(vecCreux));
    new->nbEleNN = vec->nbEleNN;
    new->taille = vec->taille;

    new->index = malloc(vec->nbEleNN * sizeof(int));
    new->val = malloc(vec->nbEleNN * sizeof(double));

    for (int i = 0; i < vec->nbEleNN; i++) {
        new->index[i] = vec->index[i];
        new->val[i] = vec->val[i];
    }

    return new;
}