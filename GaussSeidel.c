#include "GaussSeidel.h"


double calc_lambda(CSC *mtx, int *arrNN, double tol) 
{
    vecCreux *x = cree_vecteur_creux(mtx->nbColonnes, mtx->nbColonnes);
    for (int i = 0; i < x->nbEleNN; i++) {
        x->index[i] = i;
        x->val[i] = 1;
    }

    // Methode de la puissance
    double prevLambda = 0, newLambda = 0;
    for (int iter = 0; iter < 1000; iter++) {
        vecCreux *Ux = calcUx(mtx, x, arrNN);

        // Calcul (D + L)x_new = Ux
        vecCreux *nextX = linSysLxEqualb(mtx, Ux, arrNN);

        // Ux n'est plus nécessaire
        detruire_vecteur(Ux);
        if (nextX == NULL) {
            detruire_vecteur(x);
            return 0;
        }

        // Calcul de la norme d'ordre 2
        newLambda = pow(nextX->val[0], 2);
        for (int i = 1; i < nextX->nbEleNN; i++) {
            newLambda += pow(nextX->val[i], 2);
        }
        newLambda = sqrt(newLambda);

        // We only want smaller values
        if (newLambda > 1) {
            for (int i = 0; i < nextX->nbEleNN; i++) {
                nextX->val[i] /= newLambda;
            }
        }

        // Verifier la convergence
        if (fabs(newLambda - prevLambda) < tol) {
            detruire_vecteur(nextX);
            break;
        }

        // Preparation de la prochaine boucle
        prevLambda = newLambda;
        detruire_vecteur(x);
        x = nextX;
    }

    detruire_vecteur(x);
    return newLambda;
}

int GS_converge(CSC *A, int *arrNN) 
{   
    // Puisque l'on doit calculer ligne par ligne il faut retenir la somme
    double *lignSums = (double *)calloc(A->nbColonnes, sizeof(double));
    if (lignSums == NULL) {
        fprintf(stderr, "Erreur d'allocation des tableaux dans convergence\n");
        return -1;
    }
    double *diags = (double *)calloc(A->nbColonnes, sizeof(double));
    if (diags == NULL) {
        fprintf(stderr, "Erreur d'allocation des tableaux dans convergence\n");
        free(lignSums);
        return -1;
    }
    
    // Calcule des somme de ligne et de la diagonale
    for (int col = 0; col < A->nbColonnes; col++) {
        for (int idx = A->colonne[col]; idx < A->colonne[col + 1]; idx++) {
            int lgn = A->ligne[idx];

            if (lgn == col) {
                diags[col] = fabs(A->val[idx]);
            }
            else {
                lignSums[lgn] += fabs(A->val[idx]);
            }
        }

        if (diags[col] == 0) return -2; 
    }

    // Vérification de la dominance diagonale
    int conv = 1;
    for (int col = 0; col < A->nbColonnes; col++) {
        if (diags[col] < lignSums[col]) {
            conv = 0;
        }
    }
    free(lignSums);
    free(diags);

    if (conv) return 1;

    // Si la diagonale n'est pas dominante
    // Verification que la matrice est définie positive
    double l = calc_lambda(A, arrNN, 0.001);

    if (l > 0.001) {
        return 0;
    }
    return 1; 
}

vecCreux *Gauss_Seidel(CSC *A, vecCreux *b, double precision, int maxIter)
{
    // Verification que la matrice est carrée
    if (A->nbLignes != A->nbColonnes) {
        printf("La matrice A n'est pas une matrice carrée\n");
        return NULL;
    }

    // Création d'un vecteur de mémoire des indexs
    int *arrNN = (int *)malloc(b->taille * sizeof(int));
    if (arrNN == NULL) {
        fprintf(stderr, "Erreur lors de l'alocation de memoire du tableau d'indices\n");
        return NULL;
    }
    for (int i = 0; i < b->taille; i++) { arrNN[i] = -1; }

    // Pré calcule de la convergence de A
    if (!GS_converge(A, arrNN)) {
        printf("Failed Convegrence Check\n");
        free(arrNN);
        return NULL;
    }

    // Préparation du vecteur solution
    vecCreux *x = cree_vecteur_creux(b->taille, b->nbEleNN);
    if (x == NULL) {
        printf("Erreur d'allocation du vecteur solution\n");
        free(arrNN);
        return NULL;
    }
    for (int i = 0; i < b->nbEleNN; i++) {
        x->index[i] = b->index[i];
        x->val[i] = b->val[i];
    }

    // Boucle principale
    for (int iter = 0; iter < maxIter; iter++) {
        // bnew = U * x
        vecCreux *Ux = calcUx(A, x, arrNN);
        if (Ux == NULL) {
            printf("Ux != Ux at %d\n", iter);
            free(arrNN);
            detruire_vecteur(x);
            return NULL;
        }

        // Préparation des tableaux pour le vecteur
        int countNN = 0;
        int *indexTab = (int *)malloc(b->taille * sizeof(int));
        if (indexTab == NULL) {
            printf("Erreur lors de l'allocation du tableau d'index\n", iter);
            free(arrNN);
            detruire_vecteur(x);
            detruire_vecteur(Ux);
            return NULL;
        }
        double *valTab = (double *)malloc(b->taille * sizeof(double));
        if (valTab == NULL) {
            printf("Erreur lors de l'allocation du tableau de valeur\n", iter);
            free(arrNN);
            detruire_vecteur(x);
            detruire_vecteur(Ux);
            free(indexTab);
            return NULL;
        }

        // Calcul de b - Ux
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
        // Création du vecteur b - Ux 
        vecCreux *bNew = cree_vecteur_creux(b->taille, countNN);
        if (bNew == NULL) {
            printf("Failed bNew allocation at %d\n", iter);
            free(arrNN);
            detruire_vecteur(x);
            detruire_vecteur(Ux);
            free(indexTab);
            free(valTab);
            return NULL;
        }
        for (int i = 0; i < countNN; i++) {
            bNew->index[i] = indexTab[i];
            bNew->val[i] = valTab[i];
        }
        free(indexTab);
        free(valTab);
        detruire_vecteur(Ux);

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

        // Calcul de l'erreur comme max(nextX[i], x[i])
        double error = 0;
        double diff;
        for (int ptrNextX = 0, ptrX = 0; ptrNextX < nextX->nbEleNN && ptrX < x->nbEleNN; ) {
            if (nextX->index[ptrNextX] < x->index[ptrX]) {
                diff = fabs(nextX->val[ptrNextX]);
                error = fmax(error, diff);
                ptrNextX++;
            }
            else if (x->index[ptrX] < nextX->index[ptrNextX]) {
                diff = fabs(x->val[ptrX]);
                error = fmax(error, diff);
                ptrX++;
            }
            else {
                diff = fabs(x->val[ptrX] - nextX->val[ptrNextX]);
                error = fmax(error, diff);

                ptrNextX++; 
                ptrX++;
            }
        }

        // Verification de convergence
        if (error < precision) {
            detruire_vecteur(nextX);
            break; 
        } 

        detruire_vecteur(x);
        x = nextX;
    }

    free(arrNN);
    return x;
}