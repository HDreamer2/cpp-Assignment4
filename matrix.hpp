#ifndef MATRIX_HPP_
#define MATRIX_HPP_
#include <string>
class Matrix
{
private:
    int row, column;
    float **data;

public:
    Matrix();
    Matrix(int row, int column);
    Matrix(int row, int column, float **data);
    Matrix(const Matrix &B);
    ~Matrix();
    friend std::ostream &operator<<(std::ostream &os, const Matrix &A);
    Matrix operator=(const Matrix &A);
    void multikernel(float **c, float **a, float **b, int row, int col);
    Matrix operator*(const Matrix B);
    Matrix operator*(float B);
    friend Matrix operator*(float B, Matrix A);
    bool operator==(Matrix A);
};
#endif