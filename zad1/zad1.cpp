#include <iostream>
#include <limits>
#include <cmath>
#include <vector>
#include <unistd.h>

using namespace std;

float mathFunction(float x)
{
    return sin(pow(x, 3));
}

double doubleMathFunction(double x)
{
    return sin(pow(x, 3));
}

double doubleRealDeriative(double x)
{
    return 3 * pow(x, 2) * cos(pow(x, 3));
}

struct Result
{
    std::vector<double> xVals;
    std::vector<double> yVals;
};

int writePlot(const vector<double> &xValsFloat, const vector<double> &yValsFloat,
              const vector<double> &xValsDouble, const vector<double> &yValsDouble)
{
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if (!gnuplotPipe)
    {
        std::cerr << "Could not open pipe to GNUplot.\n";
        return -1;
    }

    fprintf(gnuplotPipe, "set title 'Błąd numeryczny pochodnej'\n");
    fprintf(gnuplotPipe, "set xlabel 'log10(h)'\n");
    fprintf(gnuplotPipe, "set ylabel 'log10(E(h))'\n");
    fprintf(gnuplotPipe, "set logscale x\n");
    fprintf(gnuplotPipe, "set logscale y\n");
    fprintf(gnuplotPipe, "set format x '10^{%%L}'\n");
    fprintf(gnuplotPipe, "set format y '10^{%%L}'\n");

    fprintf(gnuplotPipe, "plot '-' with lines title 'Float Data', '-' with lines title 'Double Data' \n");

    for (size_t i = 0; i < xValsFloat.size(); ++i)
    {
        fprintf(gnuplotPipe, "%e %e\n", xValsFloat[i], yValsFloat[i]);
    }
    fprintf(gnuplotPipe, "e\n");

    for (size_t i = 0; i < xValsDouble.size(); ++i)
    {
        fprintf(gnuplotPipe, "%e %e\n", xValsDouble[i], yValsDouble[i]);
    }
    fprintf(gnuplotPipe, "e\n");

    fflush(gnuplotPipe);
    pclose(gnuplotPipe);
    return 0;
}

Result floatDeriative(float x)
{
    Result result;
    float epsilon = std::numeric_limits<float>::epsilon();
    float h = 10e-20;

    while (h < 0.1)
    {
        float dhf = (mathFunction(x + h) - mathFunction(x)) / h;
        float funH = abs(dhf - doubleRealDeriative(x));
        result.xVals.push_back(h);
        result.yVals.push_back(funH);
        h *= 1.1f;
    }
    return result;
}

Result doubleDeriative(double x)
{
    Result result;
    double epsilon = std::numeric_limits<double>::epsilon();
    double h = 10e-20;

    while (h < 0.1)
    {
        double dhf = (doubleMathFunction(x + h) - doubleMathFunction(x)) / h;
        double funH = abs(dhf - doubleRealDeriative(x));
        result.xVals.push_back(h);
        result.yVals.push_back(funH);
        h *= 1.1;
    }
    return result;
}

int main()
{
    float x = 0.2f;
    Result resultFloat = floatDeriative(x);
    Result resultDouble = doubleDeriative(x);
    writePlot(resultFloat.xVals, resultFloat.yVals, resultDouble.xVals, resultDouble.yVals);
    return 0;
}
