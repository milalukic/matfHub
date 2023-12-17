#include "../include/matrix.hpp"
//TODO throw, string doens't do the job
//TODO use camelCase

#include <QStringList>

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


    // setteri

void Matrix::setData(QString data){
    int r=0, c=0;
    for(auto value : data.split(" ")){
        m_data[r][c] = value.toDouble();
        c++;
        if(c == m_columns){
            c = 0;
            r++;
        }
    }
}

void Matrix::setM1Data(QString data){
    int r=0, c=0;
    for(auto value : data.split(" ")){
        Matrix::m_M1->m_data[r][c] = value.toDouble();
        c++;
        if(c == m_M1->m_columns){
            c = 0;
            r++;
        }
    }
}
void Matrix::reshapeM1(int col, int row){
    reshapeMatrix(m_M1);
}
void Matrix::setM2Data(QString data){
    int r=0, c=0;
    for(auto value : data.split(" ")){
        m_M2->m_data[r][c] = value.toDouble();
        c++;
        if(c == m_M2->m_columns){
            c = 0;
            r++;
        }
    }
}
void Matrix::reshapeM2(int col, int row){
    reshapeMatrix(m_M2);
}

    //operators



    //functions

void Matrix::reshapeMatrix(Matrix* mat, unsigned col, unsigned row){
    delete mat->m_data;
    mat->m_data = arma::mat(col, row);
}


//TODO make this work
// std::istream &operator>>(std::istream &in, Matrix &value){

// }
