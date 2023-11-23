#include <iostream>
#include "matrica.hpp"

int main(){

    Matrica *m1 = new Matrica(3, 2, "ddd");
    std::cout << m1->sadrzaj() << std::endl;

    arma::mat kopija = m1->sadrzaj();
    kopija(0, 0) = 5;
    kopija(1, 1) = 6;
    m1->sadrzaj(kopija);

    std::cout << m1->sadrzaj()(1, 1) << std::endl;

    delete m1; m1 = nullptr;

    return 0;
}

