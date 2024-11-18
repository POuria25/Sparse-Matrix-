#ifndef MATRICE_H
#define MATRICE_H

/**
 * @file matrice.h
 * @brief Fichier d'entête du module matrice
 * @date 9 novembre 2024
 * @author KATOUZIAN Pouria
 * @author SIERRA MARIB Mateo
 *
 */

/**
 * @brief Structure de la matrice creuse
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct matrice_t
{
    unsigned int colonnes; // Nombre de colonnes
    unsigned int lignes;   // Nombre de lignes
    unsigned int elements; // Nombre d'éléments non nuls
    int *ligne;            // Tableau des lignes
    int *colonne;          // Tableau des colonnes
    double *val;           // Tableau des valeurs
} matrice;

/**
 * @brief Fonction de création de la matrice creuse
 *
 * @param file : nom du fichier contenant la matrice
 * @return CSC* : pointeur sur la matrice creuse
 */
matrice *lire_matrice(char *filename);

/**
 * @brief Fonction de destruction de la matrice
 *
 * @param ptrM : pointeur sur la matrice
 */
void detruire_matrice(matrice *ptrM);


#endif