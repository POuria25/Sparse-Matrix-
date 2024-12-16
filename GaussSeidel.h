#ifndef GAUSS_SEIDEL_H
#define GAUSS_SEIDEL_H

#include "vecteurCreux.h"
#include "csc.h"
#include "multiplication.h"
#include <math.h>

/**
 * @brief Calcule le lambda de la matrice mtx
 * @param mtx : Pointeur vers une matrice creuse
 * @param arrNN : Tableau pour retenir les indexs
 * @param tol : Tolerance pour la methode de la puissance
 * 
 * @pre mtx != NULL, arrNN est rempli de -1, tol > 0
 * 
 * @post arrNN est rempli de -1
 * 
 * @return lambda
 */
double calc_lambda(CSC *mtx, int *arrNN, double tol);

/**
 * @brief Verifie la convergence de Gauss-Seidel
 * @param A : Matrice dont on calcule la convergence
 * @param arrNN : Tableau pour retenir les indexs
 * 
 * @pre A != NULL, arrNN est rempli de -1, tol > 0
 * 
 * @post arrNN est rempli de -1
 * 
 * @return 0 si GS converge, 1 Si il diverge
 */
int GS_converge(CSC *A, int *arrNN);

/**
 * @brief Effectue la methode iterative de Gauss-Seidel
 * @param A : Pointeur vers la matrice A decomposée en (L + D) + U
 * @param b : vecteur droit de l'équation
 * @param precision : Precision minimum requise pour convergence
 * @param maxIter : Nombre maximum d'itéraTions autorisées
 * 
 * @pre A != NULL, b != NULL, precision > 0, maxIter > 0
 * 
 * @return Retourne le vecteur solution
 */
vecCreux *Gauss_Seidel(CSC *A, vecCreux *b, double precision, int maxIter);

#endif