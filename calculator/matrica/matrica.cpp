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

    //operators
//TODO make this dynamic (new doens't work)
Matrica Matrica::operator + (const Matrica &other) const{
    if(!(other.dimenzija1() == dimenzija1() && other.dimenzija2() == dimenzija2()))
        throw new std::string("Matrices are not the same dimension");
    
    arma::mat new_data = sadrzaj() + other.sadrzaj();
    Matrica new_mat = Matrica(dimenzija1(), dimenzija2(), "zbir");
    new_mat.sadrzaj(new_data);

    return new_mat;
}
Matrica Matrica::operator -(const Matrica &other) const{
    if(!(other.dimenzija1() == dimenzija1() && other.dimenzija2() == dimenzija2()))
        throw new std::string("Matrices are not the same dimension");
    
    arma::mat new_data = sadrzaj() - other.sadrzaj();
    Matrica *new_mat = new Matrica(dimenzija1(), dimenzija2(), "razlika");
    new_mat->sadrzaj(new_data);

    return *new_mat;
}
//TODO make this work
// Matrica Matrica::operator *(const Matrica &other) const{

//     if(dimenzija2() != other.dimenzija1())
//         throw new std::string("Matrices are not the right dimension");
    
//     arma::mat new_data = arma::affmul(sadrzaj(), other.sadrzaj());
//     Matrica *new_mat = new Matrica(dimenzija1(), dimenzija2(), "zbir");
//     new_mat->sadrzaj(new_data);

//     return *new_mat;
// }
// TODO make this work
// Matrica Matrica::operator /(const Matrica &other) const{
//     if(dimenzija2() != other.dimenzija1())
//         throw new std::string("Matrices are not the right dimension");
    
//     arma::mat new_data = arma::affmul(sadrzaj(), other.sadrzaj().i());
//     Matrica *new_mat = new Matrica(dimenzija1(), dimenzija2(), "zbir");
//     new_mat->sadrzaj(new_data);

//     return *new_mat;
// }
// TODO make this work
Matrica &Matrica::operator ++(){
    
    sadrzaj(sadrzaj() + 1);

    return *this;
}
//TODO make this dynamic AND make it work...
Matrica Matrica::operator ++(int){
    Matrica tmp = Matrica(dimenzija1(), dimenzija2(), "postfiksno");
    ++(*this);
    return tmp;
} 

Matrica Matrica::operator -() const{
    arma::mat new_data = sadrzaj();
    Matrica *new_mat = new Matrica(dimenzija1(), dimenzija2(), "unarni minus");
    //TODO algorithm this
    for(unsigned i = 0; i<dimenzija1(); i++)
        for(unsigned j = 0; j<dimenzija2(); j++)
            new_data(i, j) = -new_data(i, j);

    new_mat->sadrzaj(new_data);

    return *new_mat;
}

bool Matrica::operator == (const Matrica &other) const{
        if(!(other.dimenzija1() == dimenzija1() && other.dimenzija2() == dimenzija2()))
            return false;
        
        //TODO algorithm this
        for(unsigned i = 0; i<dimenzija1(); i++)
            for(unsigned j = 0; j<dimenzija2(); j++)
                if(sadrzaj()(i, j) != other.sadrzaj()(i, j))
                    return false;
        return true;
    }
    
bool Matrica::operator != (const Matrica &other) const{
        return !(*this == other);
    }

    //functions
void Matrica::transpose() {
    // *(this->_sadrzaj) = arma::trans(*(this->_sadrzaj));
    sadrzaj(arma::trans((this->sadrzaj())));
}

//TODO make this work
// void Matrica::inverse(){
//     (*this->_sadrzaj) = arma::inv(*(this->_sadrzaj));
// }

    //formatting
std::ostream &operator<<(std::ostream &out, const Matrica &value){
    return out << "\t\t" << value.imeMatrice() << "\n" << value.sadrzaj();
}
//TODO make this work
std::istream &operator>>(std::istream &in, Matrica &value){

}