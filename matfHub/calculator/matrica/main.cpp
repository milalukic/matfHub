#include <iostream>
#include "matrix.hpp"

int main(){

    // Matrix m1 = Matrix(3, 3, "dd");
    // Matrix m2 = Matrix(3, 3, "dd1");
    Matrix *m1 = new Matrix(4, 3, "ddd");
    Matrix *m2 = new Matrix(3, 3, "dddd");
    
    arma::mat kopija = m1->data();
    
    kopija(0, 0) = 5;
    kopija(1, 1) = 6;
    m1->data(kopija);
    kopija(2, 2) = 4;
    kopija(0, 1) = 6;
    m2->data(kopija);

    std::cout << *m1 << std::endl;
    m1->transpose();
    std::cout << *m1 << std::endl;

    Matrix jedinicna = Matrix(3, 3).diag();
    std::cout << jedinicna << std::endl;

    delete m1; m1 = nullptr; 
    std::cout << "EOF" << std::endl;
    return 0;
}

