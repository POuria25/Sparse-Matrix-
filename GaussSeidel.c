#include "GaussSeidel.h"

vecCreux *Gauss_Seidel(CSC *A, vecCreux *b, double precision, int maxIter)
{
    // TO DO : check convergence

    vecCreux *x = cree_vecteur_creux(b->taille, b->nbEleNN);
    if (x == NULL) {
        printf("Erreur d'allocation du vecteur solution\n");
        return NULL;
    }
    for (int i = 0; i < b->nbEleNN; i++) {
        x->index[i] = b->index[i];
        x->val[i] = b->val[i];
    }

    int *arrNN = (int *)malloc(b->taille * sizeof(int));
    if (arrNN == NULL) {
        fprintf(stderr, "Erreur lors de l'alocation de memoire du tableau d'indices\n");
        detruire_vecteur(x);
        return NULL;
    }

    for (int i = 0; i < b->taille; i++) { arrNN[i] = -1; }

    for (int iter = 0; iter < maxIter; iter++) {
        // bnew = U * x
        vecCreux *Ux = calcUx(A, x, arrNN);
        if (Ux == NULL) {
            printf("Ux != Ux at %d\n", iter);
            free(arrNN);
            detruire_vecteur(x);
            return NULL;
        }

        int countNN = 0;
        int *indexTab = (int *)malloc(b->taille * sizeof(int));
        double *valTab = (double *)malloc(b->taille * sizeof(double));
        for (int ptrB = 0, ptrUx = 0; ptrUx < Ux->nbEleNN || ptrB < b->nbEleNN;) {
            if (ptrUx >= Ux->nbEleNN) {
                indexTab[countNN] = b->index[ptrB];
                valTab[countNN] = b->val[ptrB];
                countNN++; ptrB++;
            }
            else if (ptrB >= b->nbEleNN) {
                indexTab[countNN] = Ux->index[ptrUx];
                valTab[countNN] = -Ux->val[ptrUx];
                countNN++; ptrUx++;
            }
            else {
                int indexB = b->index[ptrB];
                int indexUx = Ux->index[ptrUx];

                if (indexB == indexUx) {
                    indexTab[countNN] = indexB;
                    valTab[countNN] = b->val[ptrB] - Ux->val[ptrUx];
                    countNN++; ptrB++; ptrUx++;
                }
                else if (indexB < indexUx) {
                    indexTab[countNN] = b->index[ptrB];
                    valTab[countNN] = b->val[ptrB];
                    countNN++; ptrB++;
                }
                else {
                    indexTab[countNN] = Ux->index[ptrUx];
                    valTab[countNN] = -Ux->val[ptrUx];
                    countNN++; ptrUx++;
                }
            }
        }

        vecCreux *bNew = cree_vecteur_creux(b->taille, countNN);
        for (int i = 0; i < countNN; i++) {
            bNew->index[i] = indexTab[i];
            bNew->val[i] = valTab[i];
        }

        // (L + D) * nextX = bNew
        vecCreux *nextX = linSysLxEqualb(A, bNew, arrNN);
        if (nextX == NULL) {
            printf("Lx != b at %d\n", iter);
            free(arrNN);
            detruire_vecteur(x);
            detruire_vecteur(bNew);
            return NULL;
        }
        detruire_vecteur(bNew);

        // TO DO : verifier convergence

        detruire_vecteur(x);
        x = nextX;
    }

    return x;
}
