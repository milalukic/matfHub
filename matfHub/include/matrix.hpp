#ifndef MATRIX_HPP

#define MATRIX_HPP

#include <iostream>
#include <vector>

#include <QString>

#include <armadillo>

class Matrix{

public:
//konstruktori
    Matrix(const unsigned rows = 1, const unsigned columns = 1);
    Matrix(const unsigned rows, const unsigned columns, const QString data);
    Matrix(const Matrix &other);
    //destruktor
    ~Matrix();

    //get
    static std::pair<unsigned, unsigned> getM1shape();
    static std::pair<unsigned, unsigned> getM2shape();
    QString toString();
    static QString m1toString();
    static QString m2toString();
    static QString m3toString();
    static Matrix* getSaved(unsigned index);



    //set
    static void setM1Data(double value, unsigned i, unsigned j);
    static void reshapeM1(int col, int row);
    static void setM2Data(double value, unsigned i, unsigned j);
    static void reshapeM2(int col, int row);
    static unsigned saveMatrix();
    static std::pair<unsigned, unsigned> loadLeft(unsigned index);
    static std::pair<unsigned, unsigned> loadRight(unsigned index);

    //functions
    static bool add();
    static bool subtract();
    static bool multiply();
    static bool divide();
    static void increment();
    static void decrement();
    static void negate();

    Matrix *transpose();
    Matrix *inverse();
    Matrix *ones();
    Matrix *diag();
    //operators
    Matrix *operator + (const Matrix &other) const;
    Matrix *operator - (const Matrix &other) const;
    Matrix *operator * (const Matrix &other) const;
    Matrix *operator * (const double number) const;
    Matrix *operator-() const;

    Matrix *operator ++();
    Matrix *operator ++(int);
    Matrix *operator --();
    Matrix *operator --(int);
    Matrix *operator + (const double &number) const;
    Matrix *operator - (const double &number) const;
    Matrix *operator /(const Matrix &other) const;

    bool operator == (const Matrix &other) const;
    bool operator != (const Matrix &other) const;

    Matrix &operator = (const Matrix &other);

    
private:

    unsigned m_rows;
    unsigned m_columns;
    arma::mat* m_data;

    static Matrix* m_M1;
    static Matrix* m_M2;
    static Matrix* m_M3;

    static std::vector<Matrix*> m_savedMatrices;

    void setData(QString data);
    void reshapeMatrix(unsigned col, unsigned row);

};

std::ostream &operator<<(std::ostream &out, const Matrix *value);
std::istream &operator>>(std::istream &in, Matrix &value);

#endif //MATRIX_HPP
