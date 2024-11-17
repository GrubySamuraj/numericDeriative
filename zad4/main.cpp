#include <iostream>
#include <Eigen/Dense>
#include <cmath>

#define N 120

Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> generateMatrix(int firstValue, int secondValue)
{
    Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> matrix(N, N);
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < N; ++j)
        {
            if (i == j)
            {
                matrix(i, j) = firstValue;
            }
            else if (i == j - 1)
            {
                matrix(i, j) = secondValue;
            }
            else
            {
                matrix(i, j) = 1;
            }
        }
    }
    return matrix;
}

Eigen::VectorXd shermanMorison(Eigen::MatrixXd A1, Eigen::VectorXd u, Eigen::VectorXd v, Eigen::VectorXd b)
{
    Eigen::VectorXd z = A1 * b;           // z = A1 * b
    double vtAw = v.transpose() * A1 * u; // v^T * A1 * u
    Eigen::VectorXd w = A1 * u;           // w = A1 * u

    return z - (z * (v.transpose() * w)) / (1 + vtAw);
}
int main()
{
    Eigen::VectorXd b(N);
    Eigen::VectorXd v(N);
    Eigen::VectorXd u(N);
    Eigen::VectorXd y(N);

    for (size_t i = 0; i < N; ++i)
    {
        b(i) = 2;
        v(i) = 1;
    }
    u = v;

    Eigen::MatrixXd inversedA1(N, N); // Używamy typu MatrixXd dla zmiennoprzecinkowych wartości
    Eigen::MatrixXi A = generateMatrix(5, 3);

    inversedA1.setZero(); // Inicjalizujemy wszystkie wartości jako 0

    // Wypełnianie macierzy zgodnie z regułą
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = i; j < N; ++j)
        { // Tylko elementy na głównej przekątnej i powyżej
            double value = std::pow(-1.0, j - i) / std::pow(4.0, j - i + 1);
            inversedA1(i, j) = value;
        }
    }

    y = shermanMorison(inversedA1, u, v, b);

    std::cout
        << "wektor y: " << y.transpose() << std::endl;

    return 0;
}
