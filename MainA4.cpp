#include <iostream>
#include "matrix.hpp"

using namespace std;
int main()
{
    float **arr = new float *[2];
    for (int i = 0; i < 2; i++)
        arr[i] = new float[2];

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            arr[i][j] = i + j + 1.0;

    // cout << arr[0][0] << arr[0][1] << arr[1][0] << arr[1][1] << endl;
    Matrix m1(2, 2, arr);
    cout << m1 << endl;

    Matrix m2(m1);
    cout << m2 << endl;

    Matrix m3(2, 2);
    m3 = m1;
    cout << m3 << endl;

    Matrix m4(m1);
    cout << m4 << endl;

    m2 = m2 * m1;
    cout << m2 << endl;

    m3 = 2 * m3;
    cout << m3 << endl;

    m4 = m4 * 2;
    cout << m4 << endl;

    delete[] arr[0];
    delete[] arr[1];
    delete[] arr;
    return 0;
}
