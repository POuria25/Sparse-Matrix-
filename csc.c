
#include "csc.h"

CSC *cree_matrice_creuse(char *file)
{
    assert(file);

    matrice *ptrM = lire_matrice(file);
    if (ptrM == NULL)
    {
        fprintf(stderr, "Erreur de lecture de la matrice\n");
        return NULL;
    }

    CSC *ptrCSC = (CSC *)malloc(sizeof(CSC));
    if (ptrCSC == NULL)
    {
        fprintf(stderr, "Erreur d'allocation de la matrice creuse\n");
        detruire_matrice(ptrM);
        return NULL;
    }

    ptrCSC->nbLignes = ptrM->lignes;
    ptrCSC->nbColonnes = ptrM->colonnes;
    ptrCSC->nbElement = ptrM->elements;

    ptrCSC->ligne = (int *)malloc((ptrCSC->nbElement) * sizeof(int));
    if (ptrCSC->ligne == NULL)
    {
        fprintf(stderr, "Erreur d'allocation du tableau des lignes\n");
        detruire_matrice(ptrM);
        free(ptrCSC);
        return NULL;
    }

    ptrCSC->colonne = (int *)malloc((ptrCSC->nbColonnes + 1) * sizeof(int));
    if (ptrCSC->colonne == NULL)
    {
        fprintf(stderr, "Erreur d'allocation du tableau des colonnes\n");
        detruire_matrice(ptrM);
        free(ptrCSC->ligne);
        free(ptrCSC);
        return NULL;
    }

    ptrCSC->val = (double *)malloc(ptrCSC->nbElement * sizeof(double));
    if (ptrCSC->val == NULL)
    {
        fprintf(stderr, "Erreur d'allocation du tableau des valeurs\n");
        detruire_matrice(ptrM);
        free(ptrCSC->ligne);
        free(ptrCSC->colonne);
        free(ptrCSC);
        return NULL;
    }

    // TODO : fonction de Triage
    mergeSort(ptrM->ligne, ptrM->colonne, ptrM->val, ptrCSC->nbElement);

    int colCurrent = 0;
    int indiceColonne = 0;
    for (int i = 0; i < ptrCSC->nbElement; i++)
    {
        ptrCSC->val[i] = ptrM->val[i];
        ptrCSC->ligne[i] = ptrM->ligne[i];

        while (ptrM->colonne[i] > colCurrent)
        {
            ptrCSC->colonne[indiceColonne++] = i;
            colCurrent++;
        }
    }

    // Completer la derniere colonne
    ptrCSC->colonne[indiceColonne] = ptrCSC->nbElement;

    // Completer les colonnes restantes
    while (indiceColonne <= ptrCSC->nbElement)
    {
        ptrCSC->colonne[indiceColonne++] = ptrCSC->nbElement;
    }

    // Liberer la matrice Tmporale
    detruire_matrice(ptrM);

    return ptrCSC;
}

void print_CSC(CSC *ptrCSC)
{
    printf("Print matrice %d x %d: %d elements\n", ptrCSC->nbLignes, ptrCSC->nbColonnes, ptrCSC->nbElement);
    for (int colptr = 0; colptr < ptrCSC->nbColonnes; colptr++)
    {
        for (int eleIndex = ptrCSC->colonne[colptr]; eleIndex < ptrCSC->colonne[colptr + 1]; eleIndex++)
        {
            printf("%d %d -> %lf\n", colptr, ptrCSC->ligne[eleIndex], ptrCSC->val[eleIndex]);
        }
    }
}

void detruire_CSC(CSC *ptrCSC)
{
    free(ptrCSC->ligne);
    free(ptrCSC->colonne);
    free(ptrCSC->val);
    free(ptrCSC);
}
