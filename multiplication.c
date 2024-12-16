#include "csc.h"
#include "vecteurCreux.h"
#include "multiplication.h"

/**
 * @brief Fonction de comparaison utilisée dans le qsort de la librairie standard de c
 * @param ind1: index 1
 * @param ind2: index 2
 */
static int compare(const void *ind1, const void *ind2);

static int compare(const void *ind1, const void *ind2) {
    const int index1 = * (const int *)ind1;
    const int index2 = * (const int *)ind2;

    return index1 - index2;
}

/**
 * @brief compte le nombre de non nuls dans x, (Lx = b)
 * 
 * @param L : pointeur sur la matrice creuse triangulaire
 * @param ind : indice de x où on ajoute un element non nul
 * @param compte : pointeur vers le compte des elements non nuls de x
 * @param actifs : tableau des elements deja non nuls dans x
 */
void calcNonNulsX(CSC *L, int ind, int *compte, int *actifs, int *indexs);

void calcNonNulsX(CSC *L, int ind, int *compte, int *actifs, int *indexs) 
{
    // On ajoute que 1 fois l'indice
    if (actifs[ind] != -1) return;

    // Calcul pour cet index
    actifs[ind] = 0;
    indexs[*compte] = ind;
    (*compte)++;

    // Recursion pour les éléments de la colonne correspondante
    for (int i = L->colonne[ind]; i < L->colonne[ind + 1]; i++) {
        if (L->ligne[i] > ind)
            calcNonNulsX(L, L->ligne[i], compte, actifs, indexs);
    }
}

vecCreux *linSysLxEqualb(CSC *L, vecCreux *b, int *NNindexs) 
{
    int nonNulsX = 0;
    int *nonNulsTab = (int *)malloc(b->taille * sizeof(int));
    if (nonNulsTab == NULL) {
        fprintf(stderr, "Erreur d'allocation du tableau des indices\n");
        return NULL;
    }
    //compter les non zero du nouveau vecteur
    for (int i = 0; i < b->nbEleNN; i++) {
        calcNonNulsX(L, b->index[i], &nonNulsX, NNindexs, nonNulsTab);
    }
    if (nonNulsX == 0) {
        free(nonNulsTab);
        return NULL;
    }

    int *temp = (int *)realloc(nonNulsTab, nonNulsX * sizeof(int));
    if (temp == NULL) {
        fprintf(stderr, "Erreur d'allocation du tableau temporaire\n");
        free(nonNulsTab);
        return NULL;
    }
    nonNulsTab = temp;

    // Creation du nouveau vecteur avec le bon nombre de valeurs
    vecCreux *x = cree_vecteur_creux(b->taille, nonNulsX);
    if (x == NULL) {
        fprintf(stderr, "Erreur d'allocation du vecteur solution\n");
        for (int i = 0; i < nonNulsX; i++) {
            NNindexs[nonNulsTab[i]] = -1;
        }
        free(nonNulsTab);
        return NULL;
    }

    // Rangement des indices de x dans l'ordre
    qsort(nonNulsTab, nonNulsX, sizeof(int), compare);

    // Initialisation des valeurs et indexs de x
    for (int i = 0; i < nonNulsX; i++) {
        NNindexs[nonNulsTab[i]] = i;
        x->index[i] = nonNulsTab[i];
        x->val[i] = 0;
    }
    for (int i = 0; i < b->nbEleNN; i++) {
        x->val[NNindexs[b->index[i]]] = b->val[i];
    }
    
    // Remplissage du vecteur x
    for (int i = 0; i < x->nbEleNN; i++) {
        int index = x->index[i];

        // Recherche de la valeure diagonale
        double diag = 0;
        for (int indexElem = L->colonne[index]; indexElem < L->colonne[index + 1]; indexElem++) {
            // Si on est sur la diagonale
            if (L->ligne[indexElem] == index) {
                // Calcul xi
                diag = x->val[NNindexs[index]] / L->val[indexElem];
                x->val[NNindexs[index]] = diag;
                break;
            }
        }

        // Si bi est non nuls mais Lii l'est
        if (diag == 0) {
            detruire_vecteur(x);
            for (int j = 0; j < x->nbEleNN; j++) {
                NNindexs[nonNulsTab[j]] = -1;
            }
            free(nonNulsTab);
            return NULL;
        }

        // Calcul des xj modifier par la colonne i
        for (int indexElem = L->colonne[index]; indexElem < L->colonne[index + 1]; indexElem++) {
            // si j < i
            if (L->ligne[indexElem] <= index) continue;
            // xj = xj - Lji * xi
            x->val[NNindexs[L->ligne[indexElem]]] = x->val[NNindexs[L->ligne[indexElem]]] - L->val[indexElem] * diag;
        }
    }

    // Reinitialisation du tableau des indexs non nuls
    for (int i = 0; i < x->nbEleNN; i++) {
        NNindexs[x->index[i]] = -1;
    }

    free(nonNulsTab);
    return x;
}

vecCreux *calcUx(CSC *U, vecCreux *x, int *NNindexs)
{
    int countNN = 0;
    int *NNTab = (int *)malloc(x->taille * sizeof(int));
    // Compte non nuls
    for (int i = 0; i < x->nbEleNN; i++) {
        int col = x->index[i];

        for (int idx = U->colonne[col]; idx < U->colonne[col + 1]; idx++) {
            int lgn = U->ligne[idx];

            // Ajoute les element en haut à droite 
            if (lgn < col && NNindexs[lgn] == -1) {
                NNTab[countNN] = lgn;
                NNindexs[lgn] = 0;
                countNN++;
            }
        }
    }
    if (countNN == 0) {
        free(NNTab);
        return cree_vecteur_creux(x->taille, 0);
    }

    // Change la taille du tableau d'index
    int *temp = (int *)realloc(NNTab, countNN * sizeof(int));
    if(temp == NULL) {
        printf("Erreur realloc %d\n", countNN);
        free(NNTab);
        return NULL;
    }
    NNTab = temp;

    // Prepare le vecteur solution
    vecCreux *prod = cree_vecteur_creux(x->taille, countNN);
    if (prod == NULL) {
        printf("erreur prod\n");
        free(NNTab);
        return NULL;
    } 

    // Rangements des index dans l'ordre croissant
    qsort(NNTab, countNN, sizeof(int), compare);

    // On retient l'index des lignes de prod
    for (int i = 0; i < countNN; i++) {
        NNindexs[NNTab[i]] = i;
        prod->index[i] = NNTab[i];
        prod->val[i] = 0;
    }

    // Calcul colonne par colonne
    for (int i = 0; i < x->nbEleNN; i++) {
        int col = x->index[i];

        for (int idx = U->colonne[col]; idx < U->colonne[col + 1]; idx++) {
            int lgn = U->ligne[idx];

            if (lgn >= col) continue;

            // Ajout de la ligne dans la solution
            prod->val[NNindexs[lgn]] += U->val[idx] * x->val[i];
        }
    }

    // Reinitialisation des indexs
    for (int i = 0; i < countNN; i++) {
        NNindexs[NNTab[i]] = -1;
    }

    free(NNTab);
    return prod;
}
