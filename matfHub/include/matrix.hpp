#ifndef MATRIX_HPP

#define MATRIX_HPP

#include <iostream>
#include <vector>

#include <QString>

#include <armadillo>

class Matrix{

public:
//konstruktori
    Matrix(unsigned rows = 1, unsigned columns = 1);
    Matrix(unsigned rows, unsigned columns, QString data);
    //destruktor
    ~Matrix();

    //get



    //set
    static void setM1Data(QString data);
    static void reshapeM1(int col, int row);
    static void setM2Data(QString data);
    static void reshapeM2(int col, int row);

    //functions
    Matrix *transpose();
    Matrix *inverse();
    Matrix *ones();
    Matrix *diag();

    //operators
    Matrix *operator + (const Matrix &other) const;
    Matrix *operator + (const double &number) const;
    Matrix *operator - (const Matrix &other) const;
    Matrix *operator - (const double &number) const;
    Matrix *operator *(const Matrix &other) const;
    Matrix *operator /(const Matrix &other) const;
    Matrix *operator ++();
    Matrix *operator ++(int);
    Matrix *operator --();
    Matrix *operator --(int);
    Matrix *operator-() const;

    bool operator == (const Matrix &other) const;
    bool operator != (const Matrix &other) const;

    
private:

    unsigned m_rows;
    unsigned m_columns;
    arma::mat* m_data;

    static Matrix* m_M1;
    static Matrix* m_M2;
    static Matrix* m_M3;

    static std::vector<Matrix*> m_savedMatrices;

    void setData(QString data);
    void reshapeMatrix(Matrix* mat, unsigned col, unsigned row);

};

std::ostream &operator<<(std::ostream &out, const Matrix *value);
std::istream &operator>>(std::istream &in, Matrix &value);

#endif //MATRIX_HPP
