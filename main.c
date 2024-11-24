#include "csc.h"
#include "vecteurCreux.h"
#include "multiplication.h"

int main() {
    CSC *mtx = cree_matrice_creuse("mat.mtx");
    if (mtx == NULL) {
        printf("ERROR 1\n");
        return 0;
    }

    vecCreux *vc = lire_vecteur("vec.txt");
    if (vc == NULL) {
        detruire_CSC(mtx);
        printf("ERROR 2\n");
        return 0;
    }
    
    vecCreux *produit = multiplication_CSCtrgl_vec_creux(mtx, vc);
    if (produit == NULL) {
        detruire_CSC(mtx);
        detruire_vecteur(vc);
        printf("ERROR 3\n");
        return 0;
    }

    print_CSC(mtx);
    print_vecteur(vc);
    print_vecteur(produit);

    detruire_CSC(mtx);

    detruire_vecteur(vc);
    detruire_vecteur(produit);

    return 0;
}