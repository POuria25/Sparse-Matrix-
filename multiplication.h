#ifndef MULT_MAT_H
#define MULT_MAT_H

#include "csc.h"

/**
 * @brief Résout un système triangulaire inférieur Lx = b
 * 
 * @param L : pointeur sur la matrice creuse triangulaire inférieure
 * @param b : pointeur sur le vecteur creux
 * @param NNindexs : tableau des elements non nuls dans x
 * 
 * @pre L != NULL, b != NULL, longueur b == L->nbLignes, NNindexs rempli de -1
 * @post NNindexs rempli de -1
 * 
 * @return pointeur sur le vecteur produit x
 */
vecCreux *linSysLxEqualb(CSC *L, vecCreux *b, int *NNindexs);

/**
 * @brief Multiplie une matrice triangulaire supérieure avec un vecteur creux
 * 
 * @param U : Pointeur sur une matrice creus triangulaire supérieure
 * @param x : Pointeur sur un vecteur creux
 * @param NNindexs : tableau des elements non nuls dans x
 * 
 * @pre U != NULL, x != NULL, NNindexs rempli de -1
 * @post NNindexs rempli de -1
 * 
 * @return Pointeur vers Ux, ou NULL en cas d'erreur
 */
vecCreux *calcUx(CSC *U, vecCreux *x, int *NNindexs);

#endif