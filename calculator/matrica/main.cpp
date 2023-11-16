#include <iostream>
#include "matrica.hpp"

int main(){

    Matrica *m1 = new Matrica(5, 5, "ddd");

    delete m1; m1 = nullptr;

    return 0;
}

// //TODO imenuj
//     Matrica TransponovanaMatrica();
//     Matrica T();