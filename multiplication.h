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
vecCreux *multiplication_CSCtrgl_vec_creux(CSC *L, vecCreux *b, int *estNNVecteur);

/**
 * @brief compte le nombre de non nuls dans x, (Lx = b)
 * 
 * @param L : pointeur sur la matrice creuse triangulaire
 * @param ind : indice de x où on ajoute un element non nul
 * @param compte : pointeur vers le compte des elements non nuls de x
 * @param actifs : tableau des elements deja non nuls dans x
 */
void calcNonNulsX(CSC *L, int ind, int *compte, int *actifs, int *indexs);

#endif