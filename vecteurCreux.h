#ifndef VEC_CREUX_H
#define VEC_CREUX_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/**
 * @brief Vecteur creux
 */
typedef struct vec_creux_t
{
    int taille; /**< Taille du vecteur */
    int nbEleNN; /**< Nombre d'éléments non nuls dans le vecteur */
    int *index; /**< Indexs des valeurs non nulles du vecteur */
    double *val; /**< Valeurs non nulles du vecteur */
} vecCreux;

/**
 * @brief Fonction de creation d'un vecteur creux
 * 
 * @param taille : taille du vecteur creux
 * @param nbNN : nombre d'elements non nuls dans le vecteur
 * 
 * @return Pointeur vers un vecteur creux alloué en mémoire
 *  NULL en cas d'échec
 */
vecCreux *cree_vecteur_creux(int taille, int nbNN);

/**
 * @brief Lit un vecteur creux dans un fichier
 * 
 * @param filename : nom du fichier
 * 
 * @return Pointeur vers le vecteur creux contenu dans filename
 *  NULL en cas d'échec
 */
vecCreux *lire_vecteur(char *filename);

/**
 * @brief Affiche un vecteur creux sur le standard output
 * 
 * @param vc : pointeur vers le vecteur à afficher
 */
void print_vecteur(vecCreux *vc);

/**
 * @brief Détruit un vecteur creux
 * 
 * @param vc : Pointeur vers le vecteur creux à detruire
 */
void detruire_vecteur(vecCreux *vc);

#endif