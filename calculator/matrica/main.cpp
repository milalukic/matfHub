#include <iostream>
#include "matrica.hpp"

int main(){

    // Matrica m1 = Matrica(3, 3, "dd");
    // Matrica m2 = Matrica(3, 3, "dd1");
    Matrica *m1 = new Matrica(4, 3, "ddd");
    Matrica *m2 = new Matrica(3, 3, "dddd");
    
    arma::mat kopija = m1->sadrzaj();
    
    kopija(0, 0) = 5;
    kopija(1, 1) = 6;
    m1->sadrzaj(kopija);
    kopija(2, 2) = 4;
    kopija(0, 1) = 6;
    m2->sadrzaj(kopija);

    std::cout << *m1 << std::endl;
    m1->transpose();
    std::cout << *m1 << std::endl;

    delete m1; m1 = nullptr; 
    std::cout << "Kraj programa" << std::endl;
    return 0;
}

