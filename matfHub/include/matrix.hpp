#ifndef MATRIX_HPP

#define MATRIX_HPP

#include <iostream>
#include <vector>
#include <armadillo>

class Matrix{

public:
//konstruktori
    Matrix(unsigned dimension1 = 1, unsigned dimension2 = 1, std::string matrixName = "matrix name");
    //destruktor
    ~Matrix();

    //get
    unsigned dimension1() const;
    unsigned dimension2() const;
    std::string matrixName() const;
    arma::mat data() const;

    //set
    void dimension1(unsigned u);
    void dimension2(unsigned u);
    void matrixName(std::string name);
    void data(arma::mat Matrix);

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
    unsigned _dimension1;
    unsigned _dimension2;
    std::string _matrixName;
    arma::mat *_data;
};

std::ostream &operator<<(std::ostream &out, const Matrix *value);
std::istream &operator>>(std::istream &in, Matrix &value);

#endif //MATRIX_HPP
