#include <iostream>
#include "Matrix.h"

using namespace std;

Matrix<int> m1({
    {1,2},
    {4,5},
    {7,8} });
Matrix<int> m2(
    {
        {1,2,3},
        {4,5,6},
    });

Matrix<int>* current = &m1;
Matrix<int>* second = &m2;

void ClearScreen()
{
    system("cls");
}

void WaitForInput()
{
    cin.ignore(INT_MAX, '\n');
    cin.get();
}

void Option_Add()
{
    Matrix<int> out = *current + *second;
    if (out.IsError())
    {
        cout << "This operation is not possible!";
        return;
    }

    cout << "\nOutput:\n" << out;
}

void Option_Subtract()
{
    Matrix<int> out = *current - *second;
    if (out.IsError())
    {
        cout << "This operation is not possible!";
        return;
    }

    cout << "\nOutput:\n" << out;
}

void Option_Multiply()
{
    Matrix<int> out = *current * *second;
    if (out.IsError())
    {
        cout << "This operation is not possible!";
        return;
    }

    cout << "\nOutput:\n" << out;
}

void Option_Transpose()
{
    current->Transpose();
    if (current->IsError())
        cout << "There is an error with current matrix\n";
}

void Option_Change()
{
    ClearScreen();

    int x = 0, y = 0;
    int option = -1;

    while (true)
    {
        cout << "Current matrix:\n"<<*current<<"\n";

        cout << "Choose an option: \n"
            << "1. Change Value\n"
            << "2. Add Row\n"
            << "3. Add Col\n"
            << "4. Delete Row\n"
            << "5. Delete Col\n"
            << "0. Exit\n";

        cin >> option;
        ClearScreen();

        switch (option)
        {
        case 1:
        {
            cout << *current << "\n\n";
            cout << "Choose index [x][y] to change [starts from 0], -1 exits function\n";

            cout << "[x]";
            cin >> x;
            cout << "[y]";
            cin >> y;

            if (x == -1 || y == -1)
                return;

            if (x < 0 || y < 0 || x >= current->GetCols() || y >= current->GetRows())
            {
                cout << "x or y is invalid!";
                WaitForInput();
                ClearScreen();
                break;
            }

            cout << "New value: ";
            cin >> (*current)[y][x];
            break;
        }
        case 2:
            current->ChangeDimensions(current->GetCols(), current->GetRows()+1);
            break;
        case 3:
            current->ChangeDimensions(current->GetCols()+1, current->GetRows());
            break;
        case 4:
            current->ChangeDimensions(current->GetCols(), current->GetRows()-1);
            break;
        case 5: 
            current->ChangeDimensions(current->GetCols() - 1, current->GetRows());
            break;
        case 0:
            return;
        default:
            cout<<"Inproper input!\n";
            break;
        }

        ClearScreen();
    }
}

void Option_Swap()
{
    swap(current, second);
    cout << "Matrixes have been succesfully swapped!\n";
}


int main()
{
    cout << "Welcome to the matrix multiplication program!";
    while (true)
    {
        cout << "Your matrices:\n[Current]\n";
        cout << *current << "\n\n[Second]\n" << *second << "\n\n";

        int option = -1;
        cout << "Choose an option: \n";
        cout<< "1. Add\n"
            << "2. Subtract\n"
            << "3. Multiply\n"
            << "4. Transpose\n"
            << "5. Change\n"
            << "6. Swap\n"
            << "0. Exit\n";

        cin >> option;

        if (!cin)
        {
            cout << "Inproper input!\n";
            cin.clear();
            WaitForInput();
            ClearScreen();
            continue;
        }

        switch (option)
        {
        case 1:
            Option_Add();
            break;
        case 2:
            Option_Subtract();
            break;
        case 3:
            Option_Multiply();
            break;
        case 4:
            Option_Transpose();
            break;
        case 5:
            Option_Change();
            break;
        case 6:
            Option_Swap();
            break;
        case 0:
            return 0;
        default:
            cout << "Inproper input!";
            WaitForInput();
            ClearScreen();
            continue;
        }

        WaitForInput();
        ClearScreen();
    }
}

