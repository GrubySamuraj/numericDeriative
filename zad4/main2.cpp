
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

int main(int argc, char const *argv[])
{
    return 0;
}
