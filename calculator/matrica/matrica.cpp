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

arma::mat Matrica::sadrzaj() const{
    return *_sadrzaj;
}
    // setteri TODO

void Matrica::dimenzija1(unsigned u){
    this->_dimenzija1 = u;
}

void Matrica::dimenzija2(unsigned u){
    this->_dimenzija2 = u;
}

void Matrica::imeMatrice(std::string name){
    this->_imeMatrice = name;
}

void Matrica::sadrzaj(arma::mat matrica){
    *(this->_sadrzaj) = matrica;
}


    //functions
void Matrica::transpose() {
    *(this->_sadrzaj) = arma::trans(*(this->_sadrzaj));
}