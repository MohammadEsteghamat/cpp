#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

class Matrix
{
private:
    double matrix[5][5] = {0};

public:
    Matrix();
    bool isUpperTriangular();
    bool isLowerTriangular();
    bool isDiagonal();
    double determinant();
    int countZeros();
    bool isInvertible();
};

Matrix::Matrix()
{
    for (int8_t i = 0; i < 5; i++)
    {
        for (int8_t j = 0; j < 5; j++)
        {
            cin >> matrix[i][j];
        }
    }
}

bool Matrix::isUpperTriangular()
{
    for (int8_t i = 1; i < 5; i++)
    {
        for (int8_t j = 0; j < i; j++)
        {
            if (matrix[i][j] != 0)
                return false;
        }
    }
    return true;
}

bool Matrix::isLowerTriangular()
{
    for (int8_t i = 0; i < 5; i++)
    {
        for (int8_t j = i + 1; j < 5; j++)
        {
            if (matrix[i][j] != 0)
                return false;
        }
    }
    return true;
}

bool Matrix::isDiagonal()
{
    return isUpperTriangular() && isLowerTriangular();
}

double Matrix::determinant()
{
    double det = 1.0;
    double mat[5][5];
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            mat[i][j] = matrix[i][j];
        }
    }

    for (int i = 0; i < 5; i++)
    {
        double maxElem = abs(mat[i][i]);
        int maxRow = i;
        for (int k = i + 1; k < 5; k++)
        {
            if (abs(mat[k][i]) > maxElem)
            {
                maxElem = abs(mat[k][i]);
                maxRow = k;
            }
        }

        if (maxRow != i)
        {
            for (int j = 0; j < 5; j++)
            {
                swap(mat[maxRow][j], mat[i][j]);
            }
            det *= -1;
        }

        for (int k = i + 1; k < 5; k++)
        {
            double factor = mat[k][i] / mat[i][i];
            for (int j = i; j < 5; j++)
            {
                mat[k][j] -= factor * mat[i][j];
            }
        }
    }
    for (int i = 0; i < 5; i++)
    {
        det *= mat[i][i];
    }

    return det;
}

int Matrix::countZeros()
{
    int count{0};
    for (int8_t i = 0; i < 5; i++)
    {
        for (int8_t j = 0; j < 5; j++)
        {
            if (matrix[i][j] == 0)
                count++;
        }
    }
    return count;
}

bool Matrix::isInvertible()
{
    return determinant() != 0;
}

int main()
{
    Matrix m;
    cout << (m.isUpperTriangular() ? "The matrix is upper triangular\n" : "The matrix is not upper triangular\n");
    cout << (m.isLowerTriangular() ? "The matrix is lower triangular\n" : "The matrix is not lower triangular\n");
    cout << (m.isDiagonal() ? "The matrix is diagonal\n" : "The matrix is not diagonal\n");
    cout << fixed << setprecision(2);
    cout << "The determinant of the matrix is: " << m.determinant() << endl;
    cout << "The number of zeros in the matrix is: " << m.countZeros() << endl;
    cout << (m.isInvertible() ? "The matrix is invertible" : "The matrix is not invertible")
         << endl;

    return 0;
}
