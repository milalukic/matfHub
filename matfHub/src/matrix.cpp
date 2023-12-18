#include "../include/matrix.hpp"
//TODO throw, string doens't do the job
//TODO use camelCase

#include <QStringList>
#include <QDebug>

#define DEBUG (qDebug() << __FILE__ << ":" << __LINE__ << ":\t")

Matrix* Matrix::m_M1 = new Matrix(1,1);
Matrix* Matrix::m_M2 = new Matrix(1,1);
Matrix* Matrix::m_M3 = new Matrix(1,1);


//konstruktori
Matrix::Matrix(unsigned rows, unsigned columns):
    m_rows(rows), m_columns(columns)
{
    m_data = new arma::mat(rows, columns);
}
Matrix::Matrix(unsigned rows, unsigned columns, QString data):
    m_rows(rows), m_columns(columns)
{
    m_data = new arma::mat(rows, columns);
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

    //operators



    //functions

void Matrix::reshapeMatrix(unsigned col, unsigned row){
    arma::mat* newMat = new arma::mat(row, col);//namerno je kontra, don't worry about it
    Matrix* test = new Matrix();
    test->m_data = newMat;
    test->m_columns = col;
    test->m_rows = row;
    qDebug().noquote() << test->toString();
    newMat->fill(0);
    DEBUG << "m_columns: " << m_columns;
    DEBUG << "m_rows: " << m_rows;
    qDebug().noquote() << this->toString();
    DEBUG << "petlja";
    for(int i = 0; i < (row < m_rows ? row : m_rows); ++i){
        DEBUG << "i: " << i;
        for(int j = 0; j < (col < m_columns ? col : m_columns); ++j){
            DEBUG << "\tj: " << j;
            (*newMat)(i, j) = (*m_data)(i, j);
        }
    }
    delete m_data;
    m_data = newMat;
    m_columns = col;
    m_rows = row;
}


//TODO make this work
// std::istream &operator>>(std::istream &in, Matrix &value){

// }
