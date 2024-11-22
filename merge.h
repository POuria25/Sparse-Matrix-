#ifndef MERGE_H
#define MERGE_H


#include <assert.h>
#include <math.h> 
#include <stdio.h>
#include <stdlib.h>


/**
 * @brief Triage des élément de la matrice creuse.
 * @param lignes Tableau des lignes.
 * @param cols Tableau des colonnes.
 * @param elemnonzero Tableau des éléments non nuls.
 * @param n Taille des tableaux.
 * @return void
 */
void mergeSort(int lignes[], int cols[], double elemnonzero[], int n);

/**
 * @brief Fusion des éléments de la matrice creuse.
 * @param lignes Tableau des lignes.
 * @param cols Tableau des colonnes.
 * @param elemnonzero Tableau des éléments non nuls.
 * @param colsTmp Tableau temporaire des colonnes.
 * @param lignesTmp Tableau temporaire des lignes.
 * @param elemnonzeroTmp Tableau temporaire des éléments non nuls.
 * @param l Indice de début.
 * @param m Indice du milieu.
 * @param r Indice de fin.
 * @return void
 */
void merge(int lignes[], int cols[], double elemnonzero[], int *colsTmp,
           int *lignesTmp, double *elemnonzeroTmp, int l, int m, int r);

#endif
