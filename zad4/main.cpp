#include <iostream>
#include <Eigen/Dense>
#include <chrono>
#include <vector>

#define MAX_N 120 // Maksymalny rozmiar macierzy

// Funkcja generująca macierz zgodnie z zadaniem
Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> generateMatrix(int N, int firstValue, int secondValue)
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

// Funkcja do mierzenia czasu wykonania
template <typename Func>
double measureTime(Func func)
{
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}

int main()
{
    // Wektory przechowujące czas wykonania dla różnych metod
    std::vector<int> sizes;          // Rozmiary macierzy
    std::vector<double> customTimes; // Czas dla Twojej implementacji
    std::vector<double> luTimes;     // Czas dla LU z Eigen

    for (int N = 10; N <= MAX_N; N += 10) // Testujemy różne rozmiary macierzy
    {
        Eigen::VectorXd b(N);
        Eigen::VectorXd v(N);
        Eigen::VectorXd u(N);

        for (size_t i = 0; i < N; ++i)
        {
            b(i) = 2;
            v(i) = 1;
        }
        u = v;

        Eigen::MatrixXi A = generateMatrix(N, 5, 3);

        // Mierzenie czasu Twojej funkcji
        double customTime = measureTime([&]()
                                        {
                                            Eigen::MatrixXd inversedA1 = Eigen::MatrixXd::Zero(N, N);
                                            for (size_t i = 0; i < N; ++i)
                                            {
                                                for (size_t j = i; j < N; ++j)
                                                {
                                                    double value = std::pow(-1.0, j - i) / std::pow(4.0, j - i + 1);
                                                    inversedA1(i, j) = value;
                                                }
                                            }
                                            Eigen::VectorXd y = inversedA1 * b; });

        // Mierzenie czasu LU z Eigen
        double luTime = measureTime([&]()
                                    { Eigen::VectorXd y = A.cast<double>().lu().solve(b); });

        // Zapis wyników
        sizes.push_back(N);
        customTimes.push_back(customTime);
        luTimes.push_back(luTime);
    }

    // Tworzenie wykresu za pomocą Gnuplota
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if (!gnuplotPipe)
    {
        std::cerr << "Nie można otworzyć potoku do Gnuplota.\n";
        return 1;
    }

    fprintf(gnuplotPipe, "set title 'Porównanie czasu wykonania w zależności od N'\n");
    fprintf(gnuplotPipe, "set xlabel 'Rozmiar macierzy (N)'\n");
    fprintf(gnuplotPipe, "set ylabel 'Czas wykonania [s]'\n");
    fprintf(gnuplotPipe, "set grid\n");
    fprintf(gnuplotPipe, "set style line 1 lc rgb 'blue' lw 2 pt 7 ps 1.5\n");
    fprintf(gnuplotPipe, "set style line 2 lc rgb 'red' lw 2 pt 5 ps 1.5\n");
    fprintf(gnuplotPipe, "plot '-' using 1:2 with linespoints linestyle 1 title 'Custom Function', "
                         "'-' using 1:2 with linespoints linestyle 2 title 'LU Function'\n");

    // Dane dla Twojej implementacji
    for (size_t i = 0; i < sizes.size(); ++i)
    {
        fprintf(gnuplotPipe, "%d %f\n", sizes[i], customTimes[i]);
    }
    fprintf(gnuplotPipe, "e\n");

    // Dane dla LU z Eigen
    for (size_t i = 0; i < sizes.size(); ++i)
    {
        fprintf(gnuplotPipe, "%d %f\n", sizes[i], luTimes[i]);
    }
    fprintf(gnuplotPipe, "e\n");

    fflush(gnuplotPipe); // Wymuszenie przesłania danych do Gnuplota
    pclose(gnuplotPipe); // Zamknięcie potoku

    return 0;
}
