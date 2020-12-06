#include <iostream>
#include "matrix.hpp"

using namespace std;

Matrix::Matrix()
{
    this->row = 1;
    this->column = 1;
    data = new float *[row];
    for (int i = 0; i < row; i++)
    {
        data[i] = new float[column];
    }
}

Matrix::Matrix(int row, int column)
{
    if (!column || !row)
        return;
    this->row = row;
    this->column = column;
    data = new float *[row];
    for (int i = 0; i < row; i++)
    {
        data[i] = new float[column];
    }
}

Matrix::Matrix(int row, int column, float **data)
{
    if (!column || !row)
        return;
    this->row = row;
    this->column = column;
    this->data = (float **)malloc(row * sizeof(float *));
    float **pbr = data, **endbr = data + row, *pb, *endb,
          **par = this->data, **endar = this->data + this->row, *pa, *enda;
    do
    {
        pa = *(par++) = (float *)malloc(this->column * sizeof(float));
        enda = pa + this->column;
        pb = *(pbr++);
        endb = pb + this->column;
        do
        {
            *(pa++) = *(pb++);
        } while (pa != enda);
    } while (par != endar);
}

Matrix::Matrix(const Matrix &B)row
{
    this->row = B.row;
    this->column = B.column;
    data = (float **)malloc(row * sizeof(float *));
    float **pbr = B.data, **endbr = B.data + row, *pb, *endb,
          **par = this->data, **endar = this->data + this->row, *pa, *enda;
    do
    {
        pa = *(par++) = (float *)malloc(this->column * sizeof(float));
        enda = pa + this->column;
        pb = *(pbr++);
        endb = pb + this->column;
        do
        {
            *(pa++) = *(pb++);
        } while (pa != enda);
    } while (par != endar);
}

Matrix::~Matrix()
{
    if (!data)
        return;
    float **p = data, **end = data + row;
    do
    {
        free(*(p++));
    } while (p != end);
    column = row = 0;
    free(data);
}

std::ostream &operator<<(std::ostream &os, const Matrix &A)
{
    for (int i = 0; i < A.row; i++)
    {
        for (int j = 0; j < A.column - 1; j++)
        {
            os << A.data[i][j] << " ";
        }

        os << A.data[i][A.column - 1] << endl;
    }

    return os;
}

Matrix Matrix::operator=(const Matrix &A)
{
    if (*this == A)
        return *this;

    this->row = A.row;
    this->column = A.column;
    data = (float **)malloc(row * sizeof(float *));
    float **pbr = A.data, **endbr = A.data + row, *pb, *endb,
          **par = this->data, **endar = this->data + this->row, *pa, *enda;
    do
    {
        pa = *(par++) = (float *)malloc(this->column * sizeof(float));
        enda = pa + this->column;
        pb = *(pbr++);
        endb = pb + this->column;
        do
        {
            *(pa++) = *(pb++);
        } while (pa != enda);
    } while (par != endar);

    return *this;
}

void Matrix::multikernel(float **c, float **a, float **b, int row, int col)
{
    register float t0(0), t1(0), t2(0), t3(0), t4(0), t5(0), t6(0), t7(0),
        t8(0), t9(0), t10(0), t11(0), t12(0), t13(0), t14(0), t15(0);
    float *a0(a[0]), *a1(a[1]), *a2(a[2]), *a3(a[3]),
        *b0(b[col]), *b1(b[col + 1]), *b2(b[col + 2]), *b3(b[col + 3]), *end = b0 + row;
    do
    {
        t0 += *(a0) * *(b0);
        t1 += *(a0) * *(b1);
        t2 += *(a0) * *(b2);
        t3 += *(a0++) * *(b3);
        t4 += *(a1) * *(b0);
        t5 += *(a1) * *(b1);
        t6 += *(a1) * *(b2);
        t7 += *(a1++) * *(b3);
        t8 += *(a2) * *(b0);
        t9 += *(a2) * *(b1);
        t10 += *(a2) * *(b2);
        t11 += *(a2++) * *(b3);
        t12 += *(a3) * *(b0++);
        t13 += *(a3) * *(b1++);
        t14 += *(a3) * *(b2++);
        t15 += *(a3++) * *(b3++);
    } while (b0 != end);
    c[col][row] = t0;
    c[col + 1][row] = t1;
    c[col + 2][row] = t2;
    c[col + 3][row] = t3;
    c[col][row + 1] = t4;
    c[col + 1][row + 1] = t5;
    c[col + 2][row + 1] = t6;
    c[col + 3][row + 1] = t7;
    c[col][row + 2] = t8;
    c[col + 1][row + 2] = t9;
    c[col + 2][row + 2] = t10;
    c[col + 3][row + 2] = t11;
    c[col][row + 3] = t12;
    c[col + 1][row + 3] = t13;
    c[col + 2][row + 3] = t14;
    c[col + 3][row + 3] = t15;
}

Matrix Matrix::operator*(const Matrix B)
{
    /*
    if (this->column != B.row)
        return *this;
    Matrix tmp(this->row, B.column, 0);
    float *tr[4];
    int i(0), j(0);
    do
    {
        tr[i++] = (float *)malloc(sizeof(float) * this->column);
    } while (i < 4);
    do
    {
        i = 0;
        do
        {
            tr[0][i] = this->data[i][j]; //packing过程，把行数据打包到连续空间
            tr[1][i] = this->data[i][j + 1];
            tr[2][i] = this->data[i][j + 2];
            tr[3][i] = this->data[i][j + 3];
        } while ((++i) < this->column);
        i = 0;
        do
        {
            multikernel(tmp.data, tr, B.data, j, i);
            i += 4;
        } while (i < B.column);
        j += 4;
    } while (j < this->row);
    return tmp;
*/
    if (this->column != B.row)
    {
        cout << "Invalid input(matrix1.col != matrix2.row)" << endl;
        Matrix result = {0, 0};
        return result;
    }
    else
    {
        Matrix result = {this->row, B.column};
        int n = this->column;
        int r = result.row;
        int c = result.column;
        float s = 0;
        result.data = new float *[result.row];
        for (int i = 0; i < result.row; i++)
        {
            result.data[i] = new float[result.column];
            for (int j = 0; j < result.column; j++)
            {
                result.data[i][j] = 0;
            }
        }
        for (int i = 0; i < r; i++)
        {
            for (int k = 0; k < n; k++)
            {
                s = this->data[i][k];
                for (int j = 0; j < c; j++)
                {
                    result.data[i][j] += s * B.data[k][j];
                }
            }
        }
        return result;
    }
}

Matrix Matrix::operator*(float B)
{
    Matrix tmp(*this);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            tmp.data[i][j] *= B;
        }
    }

    return tmp;
}

Matrix operator*(float B, Matrix A)
{
    Matrix tmp(A);
    for (int i = 0; i < A.row; i++)
    {
        for (int j = 0; j < A.column; j++)
        {
            tmp.data[i][j] *= B;
        }
    }

    return tmp;
}

bool Matrix::operator==(Matrix A)
{
    if (row == A.row && column == A.column && data == A.data)
        return true;
    else
        return false;
}
