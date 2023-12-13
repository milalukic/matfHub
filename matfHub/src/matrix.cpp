#include "../include/matrix.hpp"
//TODO throw, string doens't do the job
//TODO use camelCase

//konstruktori
Matrix::Matrix(unsigned dimension1, unsigned dimension2, std::string matrixName)
    :_dimension1(dimension1),
    _dimension2(dimension2),
    _matrixName(matrixName)
{

    this->_data = new arma::mat(dimension1, dimension2);
    std::cout << "Matrix created: " << this->_matrixName << std::endl;
}
    //destruktor
Matrix::~Matrix(){
    delete this->_data; this->_data = nullptr;
    std::cout << "Matrix deleted: " << this->matrixName() << std::endl;
}

    //getteri
std::string Matrix::matrixName() const{
    return _matrixName;
}

unsigned Matrix::dimension1() const{
    return _dimension1;
}

unsigned Matrix::dimension2() const{
    return _dimension2;
}

arma::mat Matrix::data() const{
    return *_data;
}

    // setteri

void Matrix::dimension1(unsigned u){
    this->_dimension1 = u;
}

void Matrix::dimension2(unsigned u){
    this->_dimension2 = u;
}

void Matrix::matrixName(std::string name){
    this->_matrixName = name;
}

void Matrix::data(arma::mat Matrix){
    *(this->_data) = Matrix;
}

    //operators

Matrix *Matrix::operator + (const Matrix &other) const{
    if(!(other.dimension1() == dimension1() && other.dimension2() == dimension2()))
        throw new std::string("Matrices are not the same dimension");
    
    arma::mat new_data = data() + other.data();
    Matrix *new_mat = new Matrix(dimension1(), dimension2(), "zbir");
    new_mat->data(new_data);

    return new_mat;
}

Matrix *Matrix::operator + (const double &number) const{
    arma::mat new_data = data() + number;
    Matrix *new_mat = new Matrix(dimension1(), dimension2(), "zbir");
    new_mat->data(new_data);

    return new_mat;
}

Matrix *Matrix::operator -(const Matrix &other) const{
    if(!(other.dimension1() == dimension1() && other.dimension2() == dimension2()))
        throw new std::string("Matrices are not the same dimension");
    
    arma::mat new_data = data() - other.data();
    Matrix *new_mat = new Matrix(dimension1(), dimension2(), "razlika");
    new_mat->data(new_data);

    return new_mat;
}

Matrix *Matrix::operator - (const double &number) const {
    arma::mat new_data = data() - number;
    Matrix *new_mat = new Matrix(dimension1(), dimension2(), "razlika");
    new_mat->data(new_data);

    return new_mat;
}

// TODO make this work
Matrix *Matrix::operator *(const Matrix &other) const{

    if(dimension2() != other.dimension1())
        throw new std::string("Matrices are not the right dimension");

    arma::mat new_data = data() * other.data();
    Matrix *new_mat = new Matrix(other.dimension1(), dimension2(), "proizvod");
    new_mat->data(new_data);

    return new_mat;
}

Matrix *Matrix::operator /(const Matrix &other) const{

    if(dimension2() != other.dimension1())
        throw new std::string("Matrices are not the right dimension");

    if(other.dimension1() != other.dimension2())
        throw new std::string("A sqaure matrix is required");

    arma::mat inverse_mat = arma::inv(other.data());
    arma::mat new_data = data() * inverse_mat;
    Matrix *new_mat = new Matrix(other.dimension1(), dimension2(), "kolicnik");
    new_mat->data(new_data);
    return new_mat;


}

//TODO refactor, change return values to void
Matrix *Matrix::operator ++(){
    data(data() + 1);
    matrixName("inkrement");
    return this;
}

Matrix *Matrix::operator ++(int){
    data(data() + 1);
    matrixName("inkrement");
    return this;
}

Matrix *Matrix::operator --() {
    data(data() - 1);
    matrixName("umanjenje");
    return this;
}
Matrix *Matrix::operator --(int) {
    data(data() - 1);
    matrixName("umanjenje");
    return this;
}

Matrix *Matrix::operator -() const{
    arma::mat new_data = data();
    Matrix *new_mat = new Matrix(dimension1(), dimension2(), "unar minus");
    //TODO algorithm this
    for(unsigned i = 0; i<dimension1(); i++)
        for(unsigned j = 0; j<dimension2(); j++)
            new_data(i, j) = -new_data(i, j);

    new_mat->data(new_data);

    return new_mat;
}

bool Matrix::operator == (const Matrix &other) const{
        if(!(other.dimension1() == dimension1() && other.dimension2() == dimension2()))
            return false;
        
        //TODO algorithm this
        for(unsigned i = 0; i<dimension1(); i++)
            for(unsigned j = 0; j<dimension2(); j++)
                if(data()(i, j) != other.data()(i, j))
                    return false;
        return true;
    }
    
bool Matrix::operator != (const Matrix &other) const{
        return !(*this == other);
    }

    //functions
Matrix *Matrix::transpose() {
    arma::mat new_data = arma::trans(data());
    Matrix *new_mat = new Matrix(dimension2(), dimension1(), "transponovana");
    new_mat->data(new_data);
    return new_mat;
}

Matrix *Matrix::ones(){
    arma::mat new_data = arma::mat(dimension1(), dimension2()).ones();

    Matrix *new_mat = new Matrix(dimension1(), dimension2(), "Ones");
    new_mat->data(new_data);

    return new_mat;
}
Matrix *Matrix::diag(){
    if(!(dimension2() == dimension1()))
        throw new std::string("Matrices are not the same dimension");
    arma::mat new_data = arma::mat(dimension1(), dimension1()).eye();

    Matrix *new_mat = new Matrix(dimension1(), dimension2(), "E");
    new_mat->data(new_data);

    return new_mat;
}

//TODO make this work
Matrix *Matrix::inverse(){
    if(dimension1() != dimension2())
        throw new std::string("A sqaure matrix is required");

    arma::mat new_data = inv(data());
    Matrix *new_mat = new Matrix(dimension1(), dimension2(), "inverz");
    new_mat->data(new_data);
    return new_mat;
}

    //formatting
    //TODO ask about & or *
std::ostream &operator<<(std::ostream &out, const Matrix *value){
    return out << "\t\t" << value->matrixName() << "\n" << value->data();
}

//TODO make this work
// std::istream &operator>>(std::istream &in, Matrix &value){

// }
