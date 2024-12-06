#ifndef GAUSS_SEIDEL_H
#define GAUSS_SEIDEL_H

#include "vecteurCreux.h"
#include "csc.h"
#include "multiplication.h"

vecCreux *Gauss_Seidel(CSC *A, vecCreux *b, double precision, int maxIter);

#endif