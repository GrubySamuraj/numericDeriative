#include <iostream>
#include <Eigen/Dense>
#include <chrono>
#include <vector>

#define MAX_N 120

template <typename Func>
double measureTime(Func func)
{
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}

Eigen::VectorXd backSubStitution(Eigen::MatrixXd matrix, int n, Eigen::VectorXd b)
{
    Eigen::VectorXd vec = Eigen::VectorXd::Zero(n);

    for (int row = 0; row < n; ++row)
    {
        if (row == 0)
        {
            vec(row) = b(row) / matrix(row, 0);
        }
        else
        {
            vec(row) = (b(row) - vec(row - 1) * matrix(row, 1)) / matrix(row, 0);
        }
    }
    return vec;
}

Eigen::VectorXd ShermanMorrison(Eigen::MatrixXd matrix, int n, Eigen::VectorXd u, Eigen::VectorXd b, Eigen::VectorXd v)
{
    Eigen::VectorXd z = backSubStitution(matrix, n, b);
    Eigen::VectorXd q = backSubStitution(matrix, n, u);

    double scalar = 1 + v.transpose() * q;
    double scalar2 = v.transpose() * z;

    Eigen::VectorXd y = z - (scalar2 / scalar) * q;

    return y;
}

void generatePlot(std::vector<int> sizes,
                  std::vector<double> shermanMorrisonTime,
                  std::vector<double> luTimes,
                  std::vector<double> choleskyTimes,
                  std::vector<double> QRtimes)
{
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if (!gnuplotPipe)
    {
        std::cerr << "Nie można otworzyć potoku do Gnuplota.\n";
        return;
    }

    fprintf(gnuplotPipe, "set title 'Porównanie czasu wykonania w zależności od N'\n");
    fprintf(gnuplotPipe, "set xlabel 'Rozmiar macierzy (N)'\n");
    fprintf(gnuplotPipe, "set ylabel 'Czas wykonania [s]'\n");
    fprintf(gnuplotPipe, "set grid\n");
    fprintf(gnuplotPipe, "set style line 1 lc rgb 'blue' lw 2 pt 7 ps 1.5\n");
    fprintf(gnuplotPipe, "set style line 2 lc rgb 'red' lw 2 pt 5 ps 1.5\n");
    fprintf(gnuplotPipe, "set style line 3 lc rgb 'green' lw 2 pt 7 ps 1.5\n");
    fprintf(gnuplotPipe, "set style line 4 lc rgb 'purple' lw 2 pt 7 ps 1.5\n");

    fprintf(gnuplotPipe, "plot '-' using 1:2 with linespoints linestyle 1 title 'Sherman-Morrison', "
                         "'-' using 1:2 with linespoints linestyle 2 title 'LU', "
                         "'-' using 1:2 with linespoints linestyle 3 title 'Cholesky', "
                         "'-' using 1:2 with linespoints linestyle 4 title 'QR'\n");

    // Dane dla Sherman-Morrison
    for (size_t i = 0; i < sizes.size(); ++i)
    {
        fprintf(gnuplotPipe, "%d %f\n", sizes[i], shermanMorrisonTime[i]);
    }
    fprintf(gnuplotPipe, "e\n");

    // Dane dla LU
    for (size_t i = 0; i < sizes.size(); ++i)
    {
        fprintf(gnuplotPipe, "%d %f\n", sizes[i], luTimes[i]);
    }
    fprintf(gnuplotPipe, "e\n");

    // Dane dla Cholesky
    for (size_t i = 0; i < sizes.size(); ++i)
    {
        fprintf(gnuplotPipe, "%d %f\n", sizes[i], choleskyTimes[i]);
    }
    fprintf(gnuplotPipe, "e\n");

    // Dane dla QR
    for (size_t i = 0; i < sizes.size(); ++i)
    {
        fprintf(gnuplotPipe, "%d %f\n", sizes[i], QRtimes[i]);
    }
    fprintf(gnuplotPipe, "e\n");

    fflush(gnuplotPipe); // Wymuszenie przesłania danych do Gnuplota
    pclose(gnuplotPipe); // Zamknięcie potoku
}

int main()
{
    std::vector<int> sizes;
    std::vector<double> ShermanMorrisonTimes;
    std::vector<double> luTimes;
    std::vector<double> choleskyTimes;
    std::vector<double> QRtimes;

    for (int N = 10; N <= MAX_N; N += 10)
    {
        Eigen::VectorXd b(N);
        Eigen::VectorXd v(N);
        Eigen::VectorXd u(N);

        for (int i = 0; i < N; ++i)
        {
            b(i) = 2;
            v(i) = 1;
        }
        u = v;

        Eigen::MatrixXd A1 = Eigen::MatrixXd::Zero(N, 2);
        for (int i = 0; i < N; ++i)
        {
            A1(i, 0) = 4;
            if (i > 0)
                A1(i, 1) = 2;
        }
        A1(N - 1, 1) = 1;
        Eigen::VectorXd y = ShermanMorrison(A1, N, u, b, v);
        Eigen::MatrixXd A_full = Eigen::MatrixXd::Zero(N, N);
        for (size_t i = 0; i < N; ++i)
        {
            for (size_t j = 0; j < N; ++j)
            {
                if (i == j)
                {
                    A_full(i, j) = 5;
                }
                else if (i == j - 1)
                {
                    A_full(i, j) = 3;
                }
                else
                {
                    A_full(i, j) = 1;
                }
            }
        }

        double shermanTime = measureTime([&]()
                                         { Eigen::VectorXd y = ShermanMorrison(A1, N, u, b, v); 
                                             std::cout << "ShermanMorrison y: " << y << std::endl; });
        ShermanMorrisonTimes.push_back(shermanTime);

        double luTime = measureTime([&]()
                                    { Eigen::VectorXd y = A_full.lu().solve(b);
                                    std::cout << "LU y: " << y << std::endl; });
        luTimes.push_back(luTime);

        double choleskyTime = measureTime([&]()
                                          { Eigen::VectorXd y = A_full.llt().solve(b);
                                          std::cout << "Cholesky y: " << y << std::endl; });
        choleskyTimes.push_back(choleskyTime);

        double qrTime = measureTime([&]()
                                    { Eigen::VectorXd y = A_full.householderQr().solve(b);
                                    std::cout << "QR y: " << y << std::endl; });
        QRtimes.push_back(qrTime);

        sizes.push_back(N);
    }
    generatePlot(sizes, ShermanMorrisonTimes, luTimes, choleskyTimes, QRtimes);

    return 0;
}