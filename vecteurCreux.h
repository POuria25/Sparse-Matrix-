#ifndef VEC_CREUX_H
#define VEC_CREUX_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/**
 * @brief Struvture d'un vecteur creux
 */
typedef struct vec_creux_t
{
    int taille;
    int nbEleNN;
    int *index;
    double *val;
} vecCreux;

vecCreux *cree_vecteur_creux(int taille, int nbNN);

vecCreux *lire_vecteur(char *filename);

void print_vecteur(vecCreux *vc);

void detruire_vecteur(vecCreux *vc);

#endif