#ifndef CSC_H
#define CSC_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "matrice.h"

/**
 * @brief Structure de la matrice creuse au format CSR
 */
typedef struct CSC_t
{
    int nbLignes;   // Nombre de lignes
    int nbColonnes; // Nombre de colonnes
    int nbElement;  // Nombre d'éléments non nuls
    int *ligne;     // Tableau des lignes
    int *colonne;   // Tableau des colonnes
    double *val;    // Tableau des valeurs
} CSC;              // Compressed Sparse Column

/**
 * @brief Fonction de création de la matrice creuse
 *
 * @param file : nom du fichier contenant la matrice
 * @return CSC* : pointeur sur la matrice creuse
 */
CSC *cree_matrice_creuse(char *file);

/**
 * @brief Fonction de destruction de la matrice creuse
 *
 * @param ptrCSC : pointeur sur la matrice creuse
 */
void detruire_CSC(CSC *ptrCSC);

#endif