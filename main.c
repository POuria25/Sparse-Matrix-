#include "csc.h"
#include "vecteurCreux.h"
#include "multiplication.h"

int main() {
    CSC *mtx = cree_matrice_creuse("tiny.txt");
    if (mtx == NULL) {
        printf("ERROR 1\n");
        return 0;
    }

    vecCreux *vc = lire_vecteur("tiny.b.txt");
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

    vecCreux *produit = multiplication_CSCtrgl_vec_creux(mtx, vc, arrNN);
    if (produit == NULL) {
        detruire_CSC(mtx);
        detruire_vecteur(vc);
        free(arrNN);
        printf("ERROR 4\n");
        return 0;
    }

    print_CSC(mtx);
    print_vecteur(vc);
    print_vecteur(produit);

    detruire_CSC(mtx);

    detruire_vecteur(vc);
    detruire_vecteur(produit);

    free(arrNN);

    return 0;
}