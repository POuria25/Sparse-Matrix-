#include "csc.h"
#include "vecteurCreux.h"
#include "multiplication.h"

vecCreux *multiplication_CSCtrgl_vec_creux(CSC *L, vecCreux *b, int *estNNVecteur) 
{
    int nonNulsX = 0;
    int *nonNulsTab = (int *)malloc(b->taille * sizeof(int));
    if (nonNulsTab == NULL) return NULL;
    //compter les non zero du nouveau vecteur
    for (int i = 0; i < b->nbEleNN; i++) {
        calcNonNulsX(L, b->index[i], &nonNulsX, estNNVecteur, nonNulsTab);
    }
    int *temp = (int *)realloc(nonNulsTab, nonNulsX * sizeof(int));
    if (temp == NULL) {
        free(nonNulsTab);
        return NULL;
    }
    nonNulsTab = temp;

    printf("Nombre non nuls: %d\n", nonNulsX);

    //creer le nouveau vecteur avec le bon nombre de index
    vecCreux *x = cree_vecteur_creux(b->taille, nonNulsX);
    if (x == NULL) {
        for (int i = 0; i < nonNulsX; i++) {
            estNNVecteur[nonNulsTab[i]] = -1;
        }
        free(nonNulsTab);
        return NULL;
    }

    for (int i = 0; i < nonNulsX; i++) {
        printf("%d at %d\n", nonNulsTab[i],  estNNVecteur[nonNulsTab[i]]);
    }

    printf("A0\n");

    // TEMPORAIRE POUR EVITER ERREUR DANS PRINT
    for (int i = 0; i < nonNulsX; i++) {
        x->index[i] = nonNulsTab[i];
        x->val[i] = 0;
    }
    for (int i = 0; i < b->nbEleNN; i++) {
        x->val[estNNVecteur[b->index[i]]] = b->val[i];
    }
    // FIN TEMPORAIRE
    
    printf("A1\n");

    
    //remplir le vecteur x
    for (int i = 0; i < b->nbEleNN; i++) {
        int index = b->index[i];

        printf("A2\n");

        double diag = 0;
        for (int indexElem = L->colonne[index]; indexElem < L->colonne[index + 1]; indexElem++) {
            // Si Lii est non nuls
            if (L->ligne[indexElem] == index) {
                diag = b->val[i] / L->val[indexElem];
                x->index[estNNVecteur[index]] = diag;
                printf("%d * %d: %f / %f = %lf\n", L->ligne[indexElem] + 1, index + 1, b->val[i], L->val[indexElem], diag);
                break;
            }
        }

        printf("A3 %f\n",diag);

        // Si bi est non nuls mais Lii l'est
        if (diag == 0) {
            for (int j = 0; j < x->nbEleNN; j++) {
                estNNVecteur[nonNulsTab[j]] = -1;
            }
            free(nonNulsTab);
            return NULL;
        }

        printf("A4\n");

        for (int indexElem = L->colonne[index]; indexElem < L->colonne[index + 1]; indexElem++) {
            if (L->ligne[indexElem] <= index) continue;


        }
        printf("A5\n");

    }

    printf("A6\n");

    for (int i = 0; i < x->nbEleNN; i++) {
        estNNVecteur[x->index[i]] = -1;
    }

    printf("A7\n");

    return x;
}

void calcNonNulsX(CSC *L, int ind, int *compte, int *actifs, int *indexs) 
{
    if (actifs[ind] != -1) return;

    actifs[ind] = *compte;
    indexs[*compte] = ind;
    (*compte)++;

    for (int i = L->colonne[ind]; i < L->colonne[ind + 1]; i++) {
        calcNonNulsX(L, L->ligne[i], compte, actifs, indexs);
    }
}