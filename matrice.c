/**
 * @file matrice.c
 * @brief Fichier source du module matrice
 * @date 9 novembre 2024
 * @author KATOUZIAN Pouria
 * @author SIERRA MARIB Mateo
 */


#ifndef _MATRICE_H_
#define _MATRICE_H_

#include <matrice.h>

matrice *lire_matrice(char *filename)
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


void detruire_matrice(matrice *ptrM)
{
    free(ptrM->ligne);
    free(ptrM->colonne);
    free(ptrM->val);
    free(ptrM);
    return NULL;
}



#endif
