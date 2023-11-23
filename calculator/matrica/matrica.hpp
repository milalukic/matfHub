#ifndef MATRICA_HPP

#define MATRICA_HPP

#include <iostream>
#include <vector>
#include <armadillo>

class Matrica{

public:
//konstruktori
    Matrica(unsigned dimenzija1 = 1, unsigned dimenzija2 = 1, std::string imeMatrice = "ime matrice");
    //destruktor                                      
    ~Matrica();

    //get
    unsigned dimenzija1() const;
    unsigned dimenzija2() const;
    std::string imeMatrice() const;
    arma::mat sadrzaj() const;

    //set
    void dimenzija1(unsigned u);
    void dimenzija2(unsigned u);
    void imeMatrice(std::string name);
    void sadrzaj(arma::mat matrica);

    //functions
    void transpose();
    void inverse();

    //operators
    Matrica operator + (const Matrica &other) const;
    Matrica operator - (const Matrica &other) const;
    Matrica operator *(const Matrica &other) const;
    Matrica operator /(const Matrica &other) const;
    Matrica &operator ++();
    Matrica operator ++(int);  
    Matrica operator-() const;

    bool operator == (const Matrica &other) const;
    bool operator != (const Matrica &other) const;
    
private:
    unsigned _dimenzija1;
    unsigned _dimenzija2;
    std::string _imeMatrice;
    arma::mat *_sadrzaj;
};

std::ostream &operator<<(std::ostream &out, const Matrica &value);
std::istream &operator>>(std::istream &in, Matrica &value);

#endif //MATRIX_HPP