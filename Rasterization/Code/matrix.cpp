#include "matrix.h"
#include <iostream>

Matrix::Matrix(int rows, int cols) : rows(rows), cols(cols)
{
    data = new double *[rows];
    for (int i = 0; i < rows; ++i)
    {
        data[i] = new double[cols];
        for (int j = 0; j < cols; ++j)
        {
            data[i][j] = 0.0;
        }
    }
}

Matrix::~Matrix()
{
    for (int i = 0; i < rows; ++i)
    {
        delete[] data[i];
    }
    delete[] data;
}

Matrix::Matrix(const Matrix &other)
{
    rows = other.rows;
    cols = other.cols;
    data = new double *[rows];
    for (int i = 0; i < rows; ++i)
    {
        data[i] = new double[cols];
        for (int j = 0; j < cols; ++j)
            data[i][j] = other.data[i][j];
    }
}

Matrix &Matrix::operator=(const Matrix &other)
{
    if (this == &other)
        return *this;
    for (int i = 0; i < rows; ++i)
        delete[] data[i];
    delete[] data;
    rows = other.rows;
    cols = other.cols;
    data = new double *[rows];
    for (int i = 0; i < rows; ++i)
    {
        data[i] = new double[cols];
        for (int j = 0; j < cols; ++j)
            data[i][j] = other.data[i][j];
    }
    return *this;
}

void Matrix::setValue(int row, int col, double value)
{
    if (row >= 0 && row < rows && col >= 0 && col < cols)
    {
        data[row][col] = value;
    }
}

double Matrix::getValue(int row, int col) const
{
    if (row >= 0 && row < rows && col >= 0 && col < cols)
    {
        return data[row][col];
    }
    return 0.0;
}

void Matrix::constructMatrix(double **values)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            data[i][j] = values[i][j];
        }
    }
}

Matrix Matrix::operator*(const Matrix &other) const
{
    if (cols != other.rows)
    {
        throw std::invalid_argument("Matrix dimensions do not match for multiplication.");
    }
    Matrix result(rows, other.cols);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < other.cols; ++j)
        {
            for (int k = 0; k < cols; ++k)
            {
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    return result;
}
Matrix Matrix::operator+(const Matrix &other) const
{
    if (rows != other.rows || cols != other.cols)
    {
        throw std::invalid_argument("Matrix dimensions do not match for addition.");
    }
    Matrix result(rows, cols);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix &other) const
{
    if (rows != other.rows || cols != other.cols)
    {
        throw std::invalid_argument("Matrix dimensions do not match for subtraction.");
    }
    Matrix result(rows, cols);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            result.data[i][j] = data[i][j] - other.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::transpose() const
{
    Matrix result(cols, rows);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            result.data[j][i] = data[i][j];
        }
    }
    return result;
}
Matrix Matrix::identity() const
{
    if (rows != cols)
    {
        throw std::invalid_argument("Matrix must be square to create an identity matrix.");
    }

    Matrix result(rows, cols);
    for (int i = 0; i < rows; ++i)
    {
        result.data[i][i] = 1.0;
    }
    return result;
}
