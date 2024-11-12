/**
 * @file main.c
 * @brief Fichier de la fonction main
 * @date 12 novembre 2024
 * @author KATOUZIAN Pouria
 * @author SIERRA MARIB Mateo
 */

#include "matrice.h"

int main() {   
    CSC *mat = cree_matrice_creuse("mat.mtx");

    print_CSC(mat);

    detruire_CSC(mat);

    return 0;
}