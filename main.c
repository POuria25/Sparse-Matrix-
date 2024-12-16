#include "csc.h"
#include "vecteurCreux.h"
#include "multiplication.h"
#include "GaussSeidel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/**
 * @brief Validates the filename for forbidden characters.
 * @param filename The filename to validate.
 * @return 0 if valid, -1 if invalid.
 */
static int validate_filename(const char *filename)
{
    assert(filename != NULL);
    const char forbidden[] = "<>:;!|{}[]()^'\"?";
    for (size_t i = 0; i < strlen(forbidden); i++)
    {
        if (strchr(filename, forbidden[i]))
        {
            fprintf(stderr, "Error: Forbidden character in filename\n");
            return -1;
        }
    }
    return 0;
}

/**
 * @brief Prints the usage information for the program.
 * @param program_name The name of the program.
 */
static void print_usage(const char *program_name)
{
    fprintf(stdout, "Usage: %s <matrixFile> <rhsFile> <tolerance> <outputFile>\n<outputFile> is optional\n", program_name);
}

/**
 * @brief Writes the solution vector in MatrixMarket format to a file.
 * @param solution The solution vector.
 * @param filename The output filename.
 * @return 0 on success, -1 on failure.
 */
static int write_solution_to_file(const vecCreux *solution, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        fprintf(stderr, "Error: Could not open file '%s' for writing.\n", filename);
        return -1;
    }

    // Write MatrixMarket format header
    fprintf(file, "%d 1 %d\n", solution->taille, solution->nbEleNN); // Vector dimensions: size x 1

    // Write the vector elements
    for (int i = 0; i < solution->nbEleNN; i++)
    {                                                                          // Iterate over non-zero elements
        fprintf(file, "%d 1 %.10e\n", solution->index[i] + 1, solution->val[i]); // 1-based indexing
    }

    fclose(file);
    return 0;
}

/**
 * @brief Main function of the program.
 */
int main(int argv, char **args) 
{
    char *outputFile = "produit.mtx"; // default output

    if (argv < 4 || argv > 5) {
        print_usage(args[0]);
        return 0;
    }

    char *matrixFile = args[1];
    char *rhsFile = args[2];
    double tolerance = strtod(args[3], NULL);
    if (tolerance <= 0) {
        print_usage(args[0]);
        fprintf(stdout, "Tolerance shold be a number greater than 0");
        return 0;
    }

    if (argv == 5) {
        outputFile = args[4];
    }

    if (validate_filename(matrixFile) == -1 || validate_filename(rhsFile) == -1 || validate_filename(outputFile) == -1)
    {
        print_usage(args[0]);
        fprintf(stdout, "File names contain forbidden characters\n");
        return -1;
    }

    // Load the CSC matrix
    CSC *matrix = cree_matrice_creuse(matrixFile);
    if (!matrix)
    {
        fprintf(stderr, "Error: Failed to load matrix from '%s'.\n", matrixFile);
        return -1;
    }

    // Load the RHS vector
    vecCreux *rhs = lire_vecteur(rhsFile);
    if (!rhs)
    {
        fprintf(stderr, "Error: Failed to load RHS vector from '%s'.\n", rhsFile);
        detruire_CSC(matrix);
        return -1;
    }

    // Perform Gauss-Seidel method
    vecCreux *solution = Gauss_Seidel(matrix, rhs, tolerance, 1000); // Max 1000 iterations
    if (!solution)
    {
        fprintf(stderr, "Error: Gauss-Seidel method won't converge.\n");
        detruire_CSC(matrix);
        detruire_vecteur(rhs);
        return -1;
    }

    // Write the solution to the output file
    if (write_solution_to_file(solution, outputFile) != 0)
    {
        fprintf(stderr, "Error: Failed to write solution to '%s'.\n", outputFile);
        detruire_CSC(matrix);
        detruire_vecteur(rhs);
        detruire_vecteur(solution);
        return -1;
    }

    fprintf(stdout, "Solution successfully written to '%s'.\n", outputFile);

    // Clean up memory
    detruire_CSC(matrix);
    detruire_vecteur(rhs);
    detruire_vecteur(solution);
    
    return 0;
}