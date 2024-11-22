#include "merge.h"
#include <assert.h>
#include <math.h> // For fmin
#include <stdio.h>
#include <stdlib.h>

void mergeSort(int lignes[], int cols[], double elemnonzero[], int n)
{
  assert(lignes != NULL && cols != NULL && elemnonzero != NULL);

  int *colsTemp = malloc(n * sizeof(int));
  if (colsTemp == NULL)
  {
    fprintf(stderr, "Error: Memory allocation failed for temporary array.\n");
    return;
  }

  int *lignesTemp = malloc(n * sizeof(int));
  if (lignesTemp == NULL)
  {
    fprintf(stderr, "Error: Memory allocation failed for temporary array.\n");
    free(colsTemp);
    free(lignesTemp);
    return;
  }

  double *elemnonzeroTemp = malloc(n * sizeof(double));
  if (elemnonzeroTemp == NULL)
  {
    fprintf(stderr, "Error: Memory allocation failed for temporary array.\n");
    free(colsTemp);
    free(lignesTemp);
    free(elemnonzeroTemp);
    return;
  }

  int currentSize = 1;
  int leftStart;

  while (currentSize <= n - 1)
  {
    leftStart = 0;
    while (leftStart < n - 1)
    {
      int mid = fmin(leftStart + currentSize - 1, n - 1);
      int rightEnd = fmin(leftStart + 2 * currentSize - 1, n - 1);
      merge(lignes, cols, elemnonzero, colsTemp, lignesTemp, elemnonzeroTemp,
            leftStart, mid, rightEnd);
      leftStart += 2 * currentSize;
    }
    currentSize = 2 * currentSize;
  }

  if (colsTemp)
  {
    free(colsTemp);
  }
  if (lignesTemp)
  {
    free(lignesTemp);
  }
  if (elemnonzeroTemp)
  {
    free(elemnonzeroTemp);
  }
}

void merge(int lignes[], int cols[], double nnz[], int *colsTemp,
           int *lignesTemp, double *nnzTemp, int leftStart, int midPoint, int rightEnd)
{
  assert(lignes != NULL && cols != NULL && nnz != NULL);
  assert(colsTemp != NULL && lignesTemp != NULL && nnzTemp != NULL);

  // Copier les donnés aux tableaux temp tq colsTemp[] et lignesTemp[]
  int currentIndex = 1;
  while (currentIndex <= rightEnd)
  {
    colsTemp[currentIndex] = cols[currentIndex];
    lignesTemp[currentIndex] = lignes[currentIndex];
    nnzTemp[currentIndex] = nnz[currentIndex];
    currentIndex++;
  }

  // Merge temp arrays back into the original arrays
  for (int leftIndex = leftStart, rightIndex = midPoint + 1, mergedIndex = leftStart;
       leftIndex <= midPoint || rightIndex <= rightEnd;
       mergedIndex++)
  {
    /*
      Si l'index de gauche est dans la limite du point médian et (l'index de droite est 
      au-delà de la fin droite ou la valeur de colsTemp à l'index de gauche est inférieure 
      à celle à l'index de droite ou les valeurs de colsTemp aux index de gauche et de droite 
      sont égales et la valeur de lignesTemp à l'index de gauche est inférieure ou égale à 
      celle à l'index de droite)
    */
    if (leftIndex <= midPoint &&
        (rightIndex > rightEnd ||
         colsTemp[leftIndex] < colsTemp[rightIndex] ||
         (colsTemp[leftIndex] == colsTemp[rightIndex] &&
          lignesTemp[leftIndex] <= lignesTemp[rightIndex])))
    {
      // Copie l'élément du sous-tableau gauche dans le tableau original
      cols[mergedIndex] = colsTemp[leftIndex];
      lignes[mergedIndex] = lignesTemp[leftIndex];
      nnz[mergedIndex] = nnzTemp[leftIndex];
      leftIndex++;
    }
    else
    {
      // Copie l'élément du sous-tableau droit dans le tableau original
      cols[mergedIndex] = colsTemp[rightIndex];
      lignes[mergedIndex] = lignesTemp[rightIndex];
      nnz[mergedIndex] = nnzTemp[rightIndex];
      rightIndex++;
    }
  }
}
