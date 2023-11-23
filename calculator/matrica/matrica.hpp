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

    //operators
    Matrica operator + (const Matrica &other) const;
    Matrica operator - (const Matrica &other) const;
    Matrica operator *(const Matrica &other) const;
    Matrica operator /(const Matrica &other) const;
    Matrica &operator ++();
    Matrica operator ++(int);  //postfiksno mora imati tip argumenta?
    Matrica operator-() const; //unarni operator

    
    
private:
    unsigned _dimenzija1;
    unsigned _dimenzija2;
    std::string _imeMatrice;
    arma::mat *_sadrzaj;
};

#endif //MATRIX_HPP