#include <iostream>
#include "Matrix.h"

using namespace std;


Matrix<int> m1({
    {1,2},
    {4,5},
    {7,8} });
Matrix<int> m2;

int main()
{

    for (int x = 0; x < m1.GetRows(); x++)
    {
        for (int y = 0; y < m1.GetCols(); y++)
            cout << m1[x][y] << " ";
        cout << "\n";
    }

    m2 = m1.CreateTransposed();

    cout << "\n\n";

    for (int x = 0; x < m2.GetRows(); x++)
    {
        for (int y = 0; y < m2.GetCols(); y++)
            cout << m2[x][y] << " ";
        cout << "\n";
    }

    Matrix<int> mul = m1 * m2;

    cout << "\n\n";

    for (int x = 0; x < mul.GetRows(); x++)
    {
        for (int y = 0; y < mul.GetCols(); y++)
            cout << mul[x][y] << " ";
        cout << "\n";
    }
}

