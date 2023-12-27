#include "../include/matrix.hpp"
//TODO throw, string doens't do the job
//TODO use camelCase

#include <QStringList>
#include <QDebug>

#define DEBUG (qDebug() << __FILE__ << ":" << __LINE__ << ":\t")

Matrix* Matrix::m_M1 = new Matrix(0,0);
Matrix* Matrix::m_M2 = new Matrix(0,0);
Matrix* Matrix::m_M3 = new Matrix(0,0);
std::vector<Matrix*> Matrix::m_savedMatrices;


//konstruktori
Matrix::Matrix(const unsigned int rows, const unsigned int columns):
    m_rows(rows), m_columns(columns)
{
    m_data = new arma::mat(rows, columns);
}
Matrix::Matrix(const unsigned rows, const unsigned columns, const QString data):
    m_rows(rows), m_columns(columns)
{
    m_data = new arma::mat(rows, columns);
}
Matrix::Matrix(const Matrix &other):
    m_rows(other.m_rows), m_columns(other.m_columns), m_data(other.m_data)
{

}
    //destruktor
Matrix::~Matrix(){

}

    //getteri
std::pair<unsigned, unsigned> Matrix::getM1shape(){
    return {m_M1->m_columns, m_M1->m_rows};
}
std::pair<unsigned, unsigned> Matrix::getM2shape(){
    return {m_M2->m_columns, m_M2->m_rows};
}

QString Matrix::toString(){
    //DEBUG << "m_rows: " << m_rows;
    //DEBUG << "m_cols: " << m_columns;
    QString res = "";
    for(int i = 0; i < m_rows; ++i){
        res += "|\t";
        for(int j = 0; j < m_columns; ++j){
            //qDebug() << "i: " << i << " j: " << j;
            res += QString::number((*(this->m_data))(i,j)).toStdString();
            res += "\t";
        }
        res += "|\n";
    }
    return res;
}
QString Matrix::m1toString(){
    return m_M1->toString();
}
QString Matrix::m2toString(){
    return m_M2->toString();
}
QString Matrix::m3toString(){
    return m_M3->toString();
}
Matrix* Matrix::getSaved(unsigned int index){
    return m_savedMatrices[index];
}

    // setteri

void Matrix::setData(QString data){
    int r=0, c=0;
    for(auto value : data.split(" ")){
        (*(this->m_data))(r, c) = value.toDouble();
        c++;
        if(c == m_columns){
            c = 0;
            r++;
        }
    }
}

void Matrix::setM1Data(double value, unsigned i, unsigned j){
    (*(Matrix::m_M1->m_data))(i, j) = value;
}
void Matrix::reshapeM1(int col, int row){
    m_M1->reshapeMatrix(col, row);
}
void Matrix::setM2Data(double value, unsigned i, unsigned j){
    (*(Matrix::m_M2->m_data))(i, j) = value;

}
void Matrix::reshapeM2(int col, int row){
    m_M2->reshapeMatrix(col, row);
}
unsigned Matrix::saveMatrix(){
    Matrix* toSave = new Matrix(m_M3->m_rows, m_M3->m_columns);
    toSave->m_data = m_M3->m_data;
    qDebug().noquote() << toSave->toString();
    m_savedMatrices.push_back(toSave);
    return m_savedMatrices.size()-1;
}
std::pair<unsigned, unsigned> Matrix::loadLeft(unsigned index){
    Matrix* toLoad = m_savedMatrices[index];
    qDebug().noquote() << toLoad->toString();
    m_M1->reshapeMatrix(toLoad->m_columns, toLoad->m_rows);
    *m_M1 = *(toLoad);
    qDebug().noquote() << m_M1->toString();
    return {toLoad->m_columns, toLoad->m_rows};
}
std::pair<unsigned, unsigned> Matrix::loadRight(unsigned index){
    Matrix* toLoad = m_savedMatrices[index];
    qDebug().noquote() << toLoad->toString();
    m_M2->reshapeMatrix(toLoad->m_columns, toLoad->m_rows);
    *m_M2 = *(toLoad);
    qDebug().noquote() << m_M2->toString();
    return {toLoad->m_columns, toLoad->m_rows};
}


    //functions

void Matrix::reshapeMatrix(unsigned col, unsigned row){
    arma::mat* newMat = new arma::mat(row, col);//namerno je kontra, don't worry about it
    newMat->fill(0);
    qDebug().noquote() << this->toString();
    for(int i = 0; i < (row < m_rows ? row : m_rows); ++i){
        for(int j = 0; j < (col < m_columns ? col : m_columns); ++j){
            (*newMat)(i, j) = (*m_data)(i, j);
        }
    }
    delete m_data;
    m_data = newMat;
    m_columns = col;
    m_rows = row;
}

bool Matrix::add(){
    if(m_M1->m_columns != m_M2->m_columns || m_M1->m_rows != m_M2->m_rows){
        return false;
    }
    if(m_M3){
        delete m_M3;
    }
    m_M3 = *m_M1 + *m_M2;
    return true;
}
bool Matrix::subtract(){
    if(m_M1->m_columns != m_M2->m_columns || m_M1->m_rows != m_M2->m_rows){
        return false;
    }
    if(m_M3){
        delete m_M3;
    }
    m_M3 = *m_M1 - *m_M2;
    return true;
}
bool Matrix::multiply(){
    if(m_M1->m_columns != m_M2->m_rows || m_M1->m_rows != m_M2->m_columns){
        return false;
    }
    if(m_M3){
        delete m_M3;
    }
    m_M3 = (*m_M1) * (*m_M2);
    return true;
}
bool Matrix::divide(){
    return false;
}
void Matrix::increment(){

}
void Matrix::decrement(){

}
void Matrix::negate(){

}

//operators

Matrix* Matrix::operator + (const Matrix &other) const{
    Matrix* newMat = new Matrix(m_rows, m_columns);
    newMat->m_data = new arma::mat(m_rows, m_columns);
    *(newMat->m_data) = *m_data + *(other.m_data);
    return newMat;
}
Matrix* Matrix::operator - (const Matrix &other) const{
    Matrix* newMat = new Matrix(m_rows, m_columns);
    newMat->m_data = new arma::mat(m_rows, m_columns);
    *(newMat->m_data) = *m_data - *(other.m_data);
    return newMat;
}
Matrix* Matrix::operator * (const Matrix &other) const{
    Matrix* newMat = new Matrix(m_rows, other.m_columns);
    newMat->m_data = new arma::mat(m_rows, other.m_columns);
    *(newMat->m_data) = (*m_data) * (*(other.m_data));
    return newMat;
}
Matrix* Matrix::operator * (const double number) const{
    Matrix* newMat = new Matrix(m_rows, m_columns);
    newMat->m_data = new arma::mat(m_rows, m_columns);
    *(newMat->m_data) = (*m_data) * number;
    return newMat;
}
Matrix* Matrix::operator - () const{
    return *this * -1.0;
}
Matrix &Matrix::operator = (const Matrix &other) {
    m_columns = other.m_columns;
    m_rows = other.m_rows;
    m_data->reshape(m_rows, m_columns);
    *m_data = *(other.m_data);
    return *this;
}

//TODO make this work
// std::istream &operator>>(std::istream &in, Matrix &value){

// }
