#include "vecteurCreux.h"

/**
 * @brief Fonction de comparaison utilisée dans le qsort de la librairie standard de c
 */
static int compare(const void *ind1, const void *ind2);

static int compare(const void *ind1, const void *ind2) {
    const int *index1 = * (const int **)ind1;
    const int *index2 = * (const int **)ind2;

    return index1[1] - index2[1];
}

vecCreux *cree_vecteur_creux(int taille, int nbNN) 
{
    vecCreux *vc = (vecCreux *)malloc(sizeof(vecCreux));
    if (vc == NULL) {
        fprintf(stderr, "Erreur d'allocation du vecteur");
        return NULL;
    }

    if (taille > 0 && nbNN >= 0) {
        vc->taille = taille;
        vc->nbEleNN = nbNN;
    } else {
        fprintf(stderr, "Taille ou quantite de valeurs non nuls du vecteur invalide");
        free(vc);
        return NULL;
    }

    vc->index = (int *)malloc(vc->nbEleNN * sizeof(int));
    if (vc->index == NULL) {
        fprintf(stderr, "Erreur d'allocation du tableau des indexes");
        free(vc);
        return NULL;
    }

    vc->val = (double *)malloc(vc->nbEleNN * sizeof(double));
    if (vc->val == NULL) {
        fprintf(stderr, "Erreur d'allocation du tableau des valeurs");
        free(vc->index);
        free(vc);
        return NULL;
    }

    return vc;
}

vecCreux *lire_vecteur(char *filename) {
    assert(filename);

    FILE *opf = fopen(filename, "r");
    if (opf == NULL)
    {
        fprintf(stderr, "Erreur d'ouverture du fichier %s\n", filename);
        return NULL;
    }

    int taille,colonnes,nbNN;
    if (fscanf(opf, "%d %d %d", &taille, &colonnes, &nbNN) != 3)
    {
        fprintf(stderr, "Erreur de lecture des dimensions\n");
        fclose(opf);
        return NULL;
    }

    if (colonnes != 1) {
        fprintf(stderr, "La matrice fournie n'est pas un vecteur\n");
        fclose(opf);
        return NULL;
    }
    
    vecCreux *vc = cree_vecteur_creux(taille, nbNN);
    if (vc == NULL) {
        fprintf(stderr, "Erreur d'allocation du vecteur\n");
        fclose(opf);
        return NULL;
    }

    int index, colonne;
    double val;

    int countNonNuls = 0;
    for (int i = 0; i < nbNN; i++) {
        if (fscanf(opf, "%d %d %lf", &index, &colonne, &val) != 3) {
            fprintf(stderr, "Erreur de la lecture du vecteur: ");
            fclose(opf);
            detruire_vecteur(vc);
            return NULL;
        }

        if (index > vc->taille || colonne != 1) {
            fprintf(stderr, "Erreur, lecture d'un index hors du vecteur\n");
            fclose(opf);
            detruire_vecteur(vc);
            return NULL;
        }

        if (val != 0) {
            vc->index[countNonNuls] = index - 1;
            vc->val[countNonNuls] = val;
            countNonNuls++;
        }
    }

    if (countNonNuls != vc->nbEleNN) {
        vc->index = (int *)realloc(vc->index, countNonNuls * sizeof(int));
        vc->val = (double *)realloc(vc->val, countNonNuls * sizeof(double));
        vc->nbEleNN = countNonNuls;
    }

    int **memIndexs = (int **)malloc(countNonNuls * sizeof(int *));
    if (memIndexs == NULL) {
        fprintf(stderr, "Erreur d'allocation de la memoire des indexs\n");
        fclose(opf);
        detruire_vecteur(vc);
        return NULL;
    }

    double *trueVals = (double *)malloc(countNonNuls * sizeof(double));
    if (trueVals == NULL) {
        fprintf(stderr, "Erreur d'allocation des vraies positions des valeurs\n");
        fclose(opf);
        detruire_vecteur(vc);
        return NULL;
    }

    for (int i = 0; i < countNonNuls; i++) {
        memIndexs[i] = (int *)malloc(2 * sizeof(int));
        if (memIndexs[i] == NULL) {
            fprintf(stderr, "Erreur d'allocation des vraies positions des valeurs\n");
            fclose(opf);
            detruire_vecteur(vc);
            return NULL;
        }

        memIndexs[i][0] = i;
        memIndexs[i][1] = vc->index[i];
    }

    qsort(memIndexs, countNonNuls, sizeof(int *), compare);

    for (int i = 0; i < countNonNuls; i++) {
        vc->index[i] = memIndexs[i][1];
        trueVals[i] = vc->val[memIndexs[i][0]];
        free(memIndexs[i]);
    }

    free(vc->val);
    vc->val = trueVals;

    free(memIndexs);

    return vc;
}

void print_vecteur(vecCreux *vc)
{
    printf("Print vecteur de taille %d avec %d elements\n", vc->taille, vc->nbEleNN);
    for (int i = 0; i < vc->nbEleNN; i++) {
        printf("%d %lf\n", vc->index[i], vc->val[i]);
    }
}

void detruire_vecteur(vecCreux *vc)
{
    free(vc->index);
    free(vc->val);
    free(vc);
}