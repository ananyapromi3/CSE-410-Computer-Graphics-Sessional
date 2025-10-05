#pragma once
#include "vector.h"
#include <vector>

class Matrix
{
private:
    int rows;
    int cols;
    double **data;

public:
    Matrix(int rows, int cols);
    ~Matrix();
    Matrix(const Matrix &other);
    Matrix &operator=(const Matrix &other);
    void setValue(int row, int col, double value);
    void constructMatrix(double **values);
    double getValue(int row, int col) const;
    // void print() const;
    Matrix operator*(const Matrix &other) const;
    Matrix operator+(const Matrix &other) const;
    Matrix operator-(const Matrix &other) const;
    Matrix transpose() const;
    // Matrix inverse() const;
    Matrix identity() const;
};
