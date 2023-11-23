#include <iostream>
#include "matrica.hpp"

int main(){

    // Matrica m1 = Matrica(3, 3, "dd");
    // Matrica m2 = Matrica(3, 3, "dd1");
    Matrica *m1 = new Matrica(3, 3, "ddd");
    Matrica *m2 = new Matrica(3, 3, "dddd");
    
    arma::mat kopija = m1->sadrzaj();
    
    kopija(0, 0) = 5;
    kopija(1, 1) = 6;
    m1->sadrzaj(kopija);
    kopija(2, 2) = 4;
    kopija(0, 1) = 6;
    m2->sadrzaj(kopija);

    std::cout << m1->sadrzaj() << std::endl;
    std::cout << m2->sadrzaj() << std::endl;
    m1->transpose();
    Matrica m3 = -*m1 + *m1;
    Matrica m4 = *m1 + *m2;
    bool res = m3 != *m2;
    std::cout << m2->sadrzaj() << std::endl;
    std::cout << m3.sadrzaj() << std::endl;
    std::string res1 = res ? "Da" : "Ne";
    std::cout << res1 << std::endl;
    delete m1; m1 = nullptr; 
    
    std::cout << "Kraj programa" << std::endl;
    return 0;
}

