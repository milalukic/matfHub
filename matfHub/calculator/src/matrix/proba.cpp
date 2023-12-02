#include <iostream>
#include "matrix.hpp"

int main(){

    Matrix *A = new Matrix(2, 2, "matrica1");
    Matrix *B = new Matrix(2, 2, "matrica2");

    arma::mat mat1 = A->data();
    arma::mat mat2 = B->data();

    mat1(0, 0) = 1;
    mat1(0, 1) = 2;
    mat1(1, 0) = 3;
    mat1(1, 1) = 4;
    mat2(0, 0) = 4;
    mat2(0, 1) = 3;
    mat2(1, 0) = 2;
    mat2(1, 1) = 1;
    
    A->data(mat1);
    B->data(mat2);

    std::cout << *A + *B << std::endl;


    std::cout << ++(*A) << std::endl;
    std::cout << (*A)++ << std::endl;

    std::cout << --(*A) << std::endl;
    std::cout << (*A)-- << std::endl;

    std::cout << mat1 << std::endl;
    std::cout << arma::inv(mat1) << std::endl;

    std::cout << A << std::endl;
    std::cout << A->inverse() << std::endl;
    std::cout << A->transpose() << std::endl;

    std::cout << *A / *B << std::endl;

    return 0;
}