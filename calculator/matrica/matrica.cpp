#include "matrica.hpp"


//konstruktori
Matrica::Matrica(unsigned dimenzija1, unsigned dimenzija2, std::string imeMatrice)
    :_dimenzija1(dimenzija1),
    _dimenzija2(dimenzija2),
    _imeMatrice(imeMatrice)
{    

    this->_sadrzaj = new arma::mat(dimenzija1, dimenzija2);
    std::cout << "Napravljena matrica: " << this->_imeMatrice << std::endl;
}
    //destruktor
Matrica::~Matrica(){
    delete this->_sadrzaj; this->_sadrzaj = nullptr;
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

arma::mat Matrica::sadrzaj() {
    return *_sadrzaj;
}
    // setteri TODO

void Matrica::transpose() {
    *(this->_sadrzaj) = arma::trans(*(this->_sadrzaj));
}