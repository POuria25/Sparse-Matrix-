#ifndef MULT_MAT_H
#define MULT_MAT_H

#include "csc.h"

/**
 * @brief Multiplie une matrice triangulaire inférieure avec un vecteur creux
 * 
 * @param L : pointeur sur la matrice creuse triangulaire
 * @param b : pointeur sur le vecteur creux
 * @param estNNVecteur : tableau des elements non nuls dans x
 * 
 * @pre ptrCSC != NULL, vec != NULL, longueur vec == ptrCSC->nbLignes, estNNVecteur rempli de -1
 * @post estNNVecteur rempli de -1
 * 
 * @return pointeur sur le vecteur produit x
 */
vecCreux *linSysLxEqualb(CSC *L, vecCreux *b, int *estNNVecteur);

vecCreux *calcUx(CSC *U, vecCreux *x, int *NNindexs);

#endif