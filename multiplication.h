#ifndef MULT_MAT_H
#define MULT_MAT_H

#include "csc.h"

/**
 * @brief Multiplie une matrice triangulaire inférieure avec un vecteur creux
 * 
 * @param ptrCSC : pointeur sur la matrice creuse triangulaire
 * @param vec : pointeur sur le vecteur creux
 * 
 * @pre ptrCSC != NULL, vec != NULL, longueur vec == ptrCSC->nbLignes
 * 
 * @return pointeur sur le vecteur produit x
 */
vecCreux *multiplication_CSCtrgl_vec_creux(CSC *ptrCSC, vecCreux *vec);

#endif