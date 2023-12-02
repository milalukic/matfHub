#include <iostream>
#include "matrix.hpp"

int main(){

    // Matrix m1 = Matrix(3, 3, "dd");
    // Matrix m2 = Matrix(3, 3, "dd1");
    Matrix *m1 = new Matrix(2, 2, "ddd");
    Matrix *m2 = new Matrix(2, 2, "dddd");

    arma::mat mat1 = m1->data();
    arma::mat mat2 = m2->data();

    mat1(0, 0) = 1;
    mat1(0, 1) = 2;
    mat1(1, 0) = 3;
    mat1(1, 1) = 4;
    mat2(0, 0) = 4;
    mat2(0, 1) = 3;
    mat2(1, 0) = 2;
    mat2(1, 1) = 1;
    
    m1->data(mat1);
    m2->data(mat2);

    std::cout << (*m1) * (*m2) << std::endl;
    
    arma::mat kopija = m1->data();
    kopija(0, 0) = 5;
    kopija(1, 1) = 6;
    m1->data(kopija);
    kopija(2, 2) = 4;
    kopija(0, 1) = 6;
    m2->data(kopija);

    Matrix *m3 = ++m2;
    std::cout << m3 << std::endl;
    std::cout << m3 << std::endl;


    std::cout << "\tDeleting block start" << std::endl;
    delete m1; m1 = nullptr; 
    delete m3; m3 = nullptr;
    std::cout << "\tDeleting block end" << std::endl;
    
    std::cout << "EOF" << std::endl;
    return 0;
}

