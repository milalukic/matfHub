#include "matrica.hpp"


//konstruktori
Matrica::Matrica(unsigned dimenzija1, unsigned dimenzija2, std::string imeMatrice)
    :_dimenzija1(dimenzija1),
    _dimenzija2(dimenzija2),
    _imeMatrice(imeMatrice)
{    

    arma::Mat<double> *A = new arma::Mat<double>(dimenzija1, dimenzija2);
    std::cout << "Napravljena matrica: " << this->_imeMatrice << std::endl;
}
    //destruktor
Matrica::~Matrica(){
    std::cout << "Obrisana matrica: " << this->imeMatrice() << std::endl;
}

    //getteri
std::string Matrica::imeMatrice() const{
    return _imeMatrice;
}

unsigned Matrica::dimenzija1() const{
    return _dimenzija1;
}

unsigned Matrica::dimenzija2() const{
    return _dimenzija2;
}

    //setteri
