#include <iostream>
#include "matrix.hpp"

int main(){

    // Matrix m1 = Matrix(3, 3, "dd");
    // Matrix m2 = Matrix(3, 3, "dd1");
    Matrix *m1 = new Matrix(2, 2, "ddd");
    Matrix *m2 = new Matrix(2, 2, "dddd");


    std::cout << m1->data() << std::endl;

    std::cout << "\tDeleting block start" << std::endl;
    delete m1; m1 = nullptr; 
    std::cout << "\tDeleting block end" << std::endl;
    
    std::cout << "EOF" << std::endl;
    return 0;
}

