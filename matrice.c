/**
 * @file matrice.c
 * @brief Fichier source du module matrice
 * @date 12 novembre 2024
 * @author KATOUZIAN Pouria
 * @author SIERRA MARIB Mateo
 */


#ifndef _MATRICE_H_
#define _MATRICE_H_

#include "matrice.h"


/**
 * @brief Fonction de création de la matrice creuse
 * 
 * @param file : nom du fichier contenant la matrice
 * @return CSC* : pointeur sur la matrice creuse
 */
static matrice *lire_matrice(char *filename);

/**
 * @brief Fonction de destruction de la matrice
 * 
 * @param ptrM : pointeur sur la matrice
 */
static void detruire_matrice(matrice *ptrM);


static matrice *lire_matrice(char *filename)
{
    assert(filename);

    FILE *opf = fopen(filename, "r");
    if (opf == NULL)
    {
        fprintf(stderr, "Erreur d'ouverture du fichier %s\n", filename);
        return NULL;
    }

    matrice *ptrM = (matrice *)malloc(sizeof(matrice));
    if (ptrM == NULL)
    {
        fprintf(stderr, "Erreur d'allocation de la matrice\n");
        fclose(opf);
        return NULL;
    }

    if (fscanf(opf, "%d %d %d", &ptrM->lignes, &ptrM->colonnes, &ptrM->elements) != 3)
    {
        fprintf(stderr, "Erreur de lecture de la matrice\n");
        fclose(opf);
        free(ptrM);
        return NULL;
    }

    ptrM->ligne = (int *)malloc((ptrM->elements) * sizeof(int));
    if (ptrM->ligne == NULL)
    {
        fprintf(stderr, "Erreur d'allocation du tableau des lignes\n");
        fclose(opf);
        free(ptrM);
        return NULL;
    }

    ptrM->colonne = (int *)malloc((ptrM->elements) * sizeof(int));
    if (ptrM->colonne == NULL)
    {
        fprintf(stderr, "Erreur d'allocation du tableau des colonnes\n");
        fclose(opf);
        free(ptrM->ligne);
        free(ptrM);
        return NULL;
    }

    ptrM->val = (double *)malloc((ptrM->elements) * sizeof(double));
    if (ptrM->val == NULL)
    {
        fprintf(stderr, "Erreur d'allocation du tableau des valeurs\n");
        fclose(opf);
        free(ptrM->ligne);
        free(ptrM->colonne);
        free(ptrM);
        return NULL;
    }

    int elementNonZero = 0;
    for (int i = 0; i < ptrM->elements; i++)
    {
        int ligne, colonne;
        double value;

        // lire les valeurs de la matrice
        if (fscanf(opf, "%d %d %lf", &ligne, &colonne, &value) != 3)
        {
            fprintf(stderr, "Erreur de lecture de la matrice\n");
            fclose(opf);
            free(ptrM->ligne);
            free(ptrM->colonne);
            free(ptrM->val);
            free(ptrM);
            return NULL;
        }

        // Stocker les valeurs non zero dans la structure
        if (value != 0)
        {
            ptrM->ligne[elementNonZero] = ligne;
            ptrM->colonne[elementNonZero] = colonne;
            ptrM->val[elementNonZero] = value;
            elementNonZero++; // Incrementer le nombre d'elements non zero
        }
    }

    // Mettre a jour le nombre d'elements non zero
    ptrM->elements = elementNonZero;

    fclose(opf);
    return ptrM;
}

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

    ptrCSC->ligne = (int *)malloc((ptrCSC->nbColonnes + 1) * sizeof(int));
    if (ptrCSC->ligne == NULL)
    {
        fprintf(stderr, "Erreur d'allocation du tableau des lignes\n");
        detruire_matrice(ptrM);
        free(ptrCSC);
        return NULL;
    }

    ptrCSC->colonne = (int *)malloc(ptrCSC->nbElement * sizeof(int));
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

    // Liberer la matrice temporale
    detruire_matrice(ptrM);

    return ptrCSC;
}

static void detruire_matrice(matrice *ptrM)
{
    free(ptrM->ligne);
    free(ptrM->colonne);
    free(ptrM->val);
    free(ptrM);
}

void detruire_CSC(CSC *ptrCSC)
{
    free(ptrCSC->ligne);
    free(ptrCSC->colonne);
    free(ptrCSC->val);
    free(ptrCSC);
}

#endif
