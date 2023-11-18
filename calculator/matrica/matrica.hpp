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
    //getteri
    unsigned dimenzija1() const;
    unsigned dimenzija2() const;
    std::string imeMatrice() const;
    arma::mat sadrzaj();
    void transpose();

private:
    unsigned _dimenzija1;
    unsigned _dimenzija2;
    std::string _imeMatrice;
    arma::mat *_sadrzaj;
};

#endif //MATRIX_HPP