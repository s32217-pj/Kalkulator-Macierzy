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

    cout << m1;
    m2 = m1.CreateTransposed();

    cout << "\n\n";

    cout << m2;
    Matrix<int> mul = m1 * m2;

    for (auto& val : mul)
        cout << val << " ";

    cout << "\n\n";
    cout << mul;
}

