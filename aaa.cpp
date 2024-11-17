#include <iostream>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <vector>
#include <random>
#include <cmath>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_linalg.h>

const int MatrixSize = 5;

double valuesA1[] = {
    5.8267103432, 1.0419816676, 0.4517861296, -0.2246976350, 0.7150286064,
    1.0419816676, 5.8150823499, -0.8642832971, 0.6610711416, -0.3874139415,
    0.4517861296, -0.8642832971, 1.5136472691, -0.8512078774, 0.6771688230,
    -0.2246976350, 0.6610711416, -0.8512078774, 5.3014166511, 0.5228116055,
    0.7150286064, -0.3874139415, 0.6771688230, 0.5228116055, 3.5431433879};

double valuesA2[] = {
    5.4763986379, 1.6846933459, 0.3136661779, -1.0597154562, 0.0083249547,
    1.6846933459, 4.6359087874, -0.6108766748, 2.1930659258, 0.9091647433,
    0.3136661779, -0.6108766748, 1.4591897081, -1.1804364456, 0.3985316185,
    -1.0597154562, 2.1930659258, -1.1804364456, 3.3110327980, -1.1617171573,
    0.0083249547, 0.9091647433, 0.3985316185, -1.1617171573, 2.1174700695};

double valuesB[] = {
    -2.8634904630,
    -4.8216733374,
    -4.2958468309,
    -0.0877703331,
    -2.0223464006};

// Generowanie deltaB o zadanej normie
std::vector<double> generateDeltaB(std::vector<double> vectorB)
{
    const double targetNorm = 1e-6;
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);

    for (size_t i = 0; i < vectorB.size(); ++i)
    {
        vectorB[i] = distribution(generator);
    }

    double norm = std::sqrt(std::accumulate(vectorB.begin(), vectorB.end(), 0.0,
                                            [](double sum, double val)
                                            { return sum + val * val; }));

    if (norm > 0)
    {
        double scale = targetNorm / norm;
        for (auto &val : vectorB)
        {
            val *= scale;
        }
    }

    return vectorB;
}

// Rozwiązywanie macierzy
void solveMatrix(gsl_matrix *matrix, gsl_permutation *p, gsl_vector *x, gsl_vector *B)
{
    int signum;
    gsl_linalg_LU_decomp(matrix, p, &signum);
    gsl_linalg_LU_solve(matrix, p, B, x);
}

int main()
{
    std::vector<double> valuesDeltaB = generateDeltaB(std::vector<double>(MatrixSize));

    gsl_matrix_view matrixA1_view = gsl_matrix_view_array(valuesA1, MatrixSize, MatrixSize);
    gsl_matrix_view matrixA2_view = gsl_matrix_view_array(valuesA2, MatrixSize, MatrixSize);

    gsl_vector_view B = gsl_vector_view_array(valuesB, MatrixSize);
    gsl_vector_view deltaB = gsl_vector_view_array(valuesDeltaB.data(), MatrixSize);

    gsl_vector *x1 = gsl_vector_alloc(MatrixSize);
    gsl_vector *deltax1 = gsl_vector_alloc(MatrixSize);
    gsl_vector *x2 = gsl_vector_alloc(MatrixSize);
    gsl_vector *deltax2 = gsl_vector_alloc(MatrixSize);

    gsl_permutation *p = gsl_permutation_alloc(MatrixSize);

    // Alokacja i kopiowanie macierzy
    gsl_matrix *matrixA1 = gsl_matrix_alloc(MatrixSize, MatrixSize);
    gsl_matrix *matrixA2 = gsl_matrix_alloc(MatrixSize, MatrixSize);
    gsl_matrix_memcpy(matrixA1, &matrixA1_view.matrix);
    gsl_matrix_memcpy(matrixA2, &matrixA2_view.matrix);

    // Rozwiązywanie układów równań
    solveMatrix(matrixA1, p, x1, &B.vector);
    solveMatrix(matrixA1, p, deltax1, &deltaB.vector);

    gsl_matrix_memcpy(matrixA1, &matrixA1_view.matrix); // Przywrócenie macierzy
    solveMatrix(matrixA2, p, x2, &B.vector);
    solveMatrix(matrixA2, p, deltax2, &deltaB.vector);

    // Wyświetlanie wyników
    std::cout << "Rozwiązanie x1:\n";
    for (int i = 0; i < MatrixSize; ++i)
    {
        std::cout << gsl_vector_get(x1, i) << " ";
    }
    std::cout << "\nRozwiązanie deltax1:\n";
    for (int i = 0; i < MatrixSize; ++i)
    {
        std::cout << gsl_vector_get(deltax1, i) << " ";
    }
    std::cout << "\nRozwiązanie x2:\n";
    for (int i = 0; i < MatrixSize; ++i)
    {
        std::cout << gsl_vector_get(x2, i) << " ";
    }
    std::cout << "\nRozwiązanie deltax2:\n";
    for (int i = 0; i < MatrixSize; ++i)
    {
        std::cout << gsl_vector_get(deltax2, i) << " ";
    }
    std::cout << std::endl;

    // Zwalnianie pamięci
    gsl_permutation_free(p);
    gsl_vector_free(x1);
    gsl_vector_free(deltax1);
    gsl_vector_free(x2);
    gsl_vector_free(deltax2);
    gsl_matrix_free(matrixA1);
    gsl_matrix_free(matrixA2);

    return 0;
}
