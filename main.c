#include "csc.h"
#include "vecteurCreux.h"
#include "multiplication.h"
#include "GaussSeidel.h"
#include <stdio.h>

int main(int argv, char **args) {
    char *filename = "tiny.txt";
    if (argv > 1) filename = args[1];

    CSC *mtx = cree_matrice_creuse(filename);
    if (mtx == NULL) {
        printf("ERROR 1\n");
        return 0;
    }

    filename = "tiny.b.txt";
    if (argv > 2) filename = args[2];

    vecCreux *vc = lire_vecteur(filename);
    if (vc == NULL) {
        detruire_CSC(mtx);
        printf("ERROR 2\n");
        return 0;
    }

    int *arrNN = (int *)malloc(vc->taille * sizeof(int));
    if (arrNN == NULL) {
        detruire_CSC(mtx);
        detruire_vecteur(vc);
        printf("ERROR 3\n");
        return 0;
    }

    for (int i = 0; i < vc->taille; i++) { arrNN[i] = -1; }

    double conv = 0.000001;
    if (argv > 3) conv = strtod(args[3], &filename);
    if (*filename != '\0') conv = 0.01;

    vecCreux *produit = Gauss_Seidel(mtx, vc, conv, 100);
    if (produit == NULL) {
        detruire_CSC(mtx);
        detruire_vecteur(vc);
        free(arrNN);
        printf("ERROR 4\n");
        return 0;
    }

    print_vecteur(produit);

    detruire_CSC(mtx);

    detruire_vecteur(vc);
    detruire_vecteur(produit);

    free(arrNN);

    return 0;
}