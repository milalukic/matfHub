#include <iostream>
#include "matrica.hpp"

int main(){

    Matrica *m1 = new Matrica(3, 2, "ddd");
    std::cout << m1->sadrzaj() << std::endl;
    m1->transpose();
    std::cout << m1->sadrzaj() << std::endl;

    delete m1; m1 = nullptr;

    return 0;
}

// //TODO imenuj
//     Matrica TransponovanaMatrica();
//     Matrica T();