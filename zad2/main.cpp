#include <Eigen/Dense>
#include <iostream>
#include <random>
#include <iomanip>

Eigen::VectorXd solveMatrix(const Eigen::MatrixXd &matrix, const Eigen::VectorXd &vector)
{
    Eigen::LLT<Eigen::MatrixXd> cholesky(matrix);
    Eigen::VectorXd result;
    if (cholesky.info() == Eigen::Success)
    {
        result = cholesky.solve(vector);
    }
    else
    {
        throw "Faktoryzacja Cholesky'ego nie powiodła się. Macierz nie jest dodatnio określona.";
    }
    return result;
}

Eigen::VectorXd generateDeltaB(int size, double targetNorm = 1e-6)
{
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);

    Eigen::VectorXd deltaB(size);
    for (int i = 0; i < size; ++i)
    {
        deltaB(i) = distribution(generator);
    }

    deltaB *= targetNorm / deltaB.norm();

    return deltaB;
}

int main()
{
    Eigen::Matrix<double, 5, 5> A1;
    A1 << 5.8267103432, 1.0419816676, 0.4517861296, -0.2246976350, 0.7150286064, 1.0419816676, 5.8150823499, -0.8642832971, 0.6610711416, -0.3874139415, 0.4517861296, -0.8642832971, 1.5136472691, -0.8512078774, 0.6771688230, -0.2246976350, 0.6610711416, -0.8512078774, 5.3014166511, 0.5228116055, 0.7150286064, -0.3874139415, 0.6771688230, 0.5228116055, 3.5431433879;

    Eigen::Matrix<double, 5, 5> A2;
    A2 << 5.4763986379, 1.6846933459, 0.3136661779, -1.0597154562, 0.0083249547,
        1.6846933459, 4.6359087874, -0.6108766748, 2.1930659258, 0.9091647433,
        0.3136661779, -0.6108766748, 1.4591897081, -1.1804364456, 0.3985316185,
        -1.0597154562, 2.1930659258, -1.1804364456, 3.3110327980, -1.1617171573,
        0.0083249547, 0.9091647433, 0.3985316185, -1.1617171573, 2.1174700695;

    Eigen::Vector<double, 5> b;
    b << -2.8634904630,
        -4.8216733374,
        -4.2958468309,
        -0.0877703331,
        -2.0223464006;

    Eigen::VectorXd deltaB = generateDeltaB(5);
    Eigen::VectorXd x1 = solveMatrix(A1, b);
    Eigen::VectorXd x2 = solveMatrix(A2, b);
    Eigen::VectorXd deltax1 = solveMatrix(A1, b + deltaB);
    Eigen::VectorXd deltax2 = solveMatrix(A2, b + deltaB);

    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver1(A1.transpose() * A1);
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver2(A2.transpose() * A2);

    double max_eigenvalue = solver1.eigenvalues().maxCoeff();
    double min_eigenvalue = solver1.eigenvalues().minCoeff();

    double max_eigenValueA2 = solver2.eigenvalues().maxCoeff();
    double min_eigenValueA2 = solver2.eigenvalues().minCoeff();

    double condition_numberA1 = abs(max_eigenvalue) / abs(min_eigenvalue);
    double condition_numberA2 = abs(max_eigenValueA2) / abs(min_eigenValueA2);

    std::setprecision(15);
    std::cout << "Rozwiązanie dla A1 * x = b:\n"
              << x1.transpose() << "\n";
    std::cout << "Rozwiązanie zaburzone dla A1 * x = b + deltaB:\n"
              << deltax1.transpose() << "\n";
    std::cout << "Rozwiązanie dla A2 * x = b:\n"
              << x2.transpose() << "\n";
    std::cout << "Rozwiązanie zaburzone dla A2 * x = b + deltaB:\n"
              << deltax2.transpose() << "\n";
    std::cout << "Roznica x1: " << (x1 - deltax1).transpose() << std::endl;
    std::cout << "Roznica x2: " << (x2 - deltax2).transpose() << std::endl;
    std::cout << "Wskaźnik uwarunkowania macierzy A1: " << condition_numberA1 << std::endl;
    std::cout << "Wskaźnik uwarunkowania macierzy A2: " << condition_numberA2 << std::endl;

    return 0;
}
