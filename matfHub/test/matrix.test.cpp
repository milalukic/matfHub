#define CATCH_CONFIG_MAIN


#include "../catch.hpp"
#include "../include/matrix.hpp"

TEST_CASE("Matrix calculator", "[class]"){

    SECTION("Getter getShape returns dimensions of matrix"){
        Matrix* matrix = new Matrix(3, 5);

        auto [x, y] = matrix->getShape();
        bool resultCorrect = (x == 3) && (y == 5);

    	REQUIRE(resultCorrect);
    }

    SECTION("Method toString returns nicely fomratted string of the matrix"){
    	Matrix* matrix = new Matrix(2, 2);

    	QString expected = "|\t1\t|\n";
    	auto output = matrix->toString();

    	REQUIRE(QString::compare(output, expected));
    }

    SECTION("Getters rows and columns return dimensions of matrix, should be the same as getShape"){
    	Matrix* matrix = new Matrix(3, 5);
    	auto [x, y] = matrix->getShape();
    	
    	unsigned x1 = matrix->rows();
    	unsigned y1 = matrix->columns();
    	bool resultCorrect = (x == x1) && (y = y1);

    	REQUIRE(resultCorrect);
    }

    SECTION("Setter setData sets values of matrix from QString"){
    	Matrix* matrix = new Matrix(1, 1);
    	QString dataString = "5";

    	matrix->setData(dataString);

    	REQUIRE(((matrix->data())(0, 0)) == 5);
    }

    SECTION("Method reshapeMatrix reshapes the matrix and sets its dimensions"){
    	Matrix* matrix = new Matrix(1, 2);
    	matrix->setValue(5, 0, 0);

    	matrix->reshapeMatrix(2, 1);
    	auto [x, y] = matrix->getShape();
    	auto val1 = (matrix->data())(0, 0);
    	auto val2 = (matrix->data())(0, 1);
    	bool expected = (x == 1) && (y == 2) && (val1 == 5) && (val2 == 0);
 
    	REQUIRE(expected);
    }
    
    SECTION("Method getSaved returns saved matrix"){
    	Matrix* matrix = new Matrix(1, 2);
    	matrix->setValue(5, 0, 0);
    	
    	matrix->saveMatrix();	
    	auto savedMatrix = matrix->getSaved(0);   	
    	bool expected = ((savedMatrix->data())(0,0)==5);
    	
    	REQUIRE(expected);
    }
    
    SECTION("Method getSaved doesn't return null when there are no saved matrices"){
    	Matrix* matrix = new Matrix(1, 2);
    	matrix->setValue(5, 0, 0);
   	
    	auto expected = matrix->getSaved(0);
    	
    	REQUIRE(expected != nullptr);
    }
    
    SECTION("Method data returns the number in the given row and column"){
    	Matrix* matrix = new Matrix(1, 2);
    	matrix->setValue(5, 0, 0);
    	
    	bool expected = (matrix->data()(0,0) == 5) && (matrix->data()(0,1) == 0);
    	
    	REQUIRE(expected);
    }
    
    SECTION("Method setValue sets the given value in the given row and column"){
    	Matrix* matrix = new Matrix(1, 2);
    	matrix->setValue(5, 0, 1);
    	
    	bool expected = matrix->data()(0,1) == 5;
    	
    	REQUIRE(expected);
    }
    
    SECTION("Method switchMatrices switches the shapes of two matrices"){
    	Matrix* matrix1 = new Matrix(1, 1);
    	Matrix* matrix2 = new Matrix(1, 2);
    	Matrix::switchMatrices(matrix1, matrix2);    	
    	auto [x1, y1] = matrix1->getShape();
    	auto [x2, y2] = matrix2->getShape();
    	
    	bool expected = (x1 == 1) && (y1 == 2) && (x2 == 1) && (x2 == 1);
    	
    	REQUIRE(expected);
    }
   
   SECTION("Method switchMatrices switches the data of two matrices"){
   	Matrix* matrix1 = new Matrix(1, 1);
    	Matrix* matrix2 = new Matrix(1, 2);
    	matrix1->setValue(5, 0, 0);
    	Matrix::switchMatrices(matrix1, matrix2); 
    	
    	bool expected = (matrix2->data()(0,0) == 5) && (matrix1->data()(0,0) == 0) && (matrix1->data()(0,1) == 0);
    	
    	REQUIRE(expected);
   }
   
   SECTION("Method rows sets the number of rows of a matrix"){
   	Matrix* matrix = new Matrix(1, 1);
   	matrix->rows(2);
   	auto [x, y] = matrix->getShape();
   	
   	bool expected = (x == 2) && (y==1);
   	
   	REQUIRE(expected);
   }
   
   SECTION("Method columns sets the number of columns of a matrix"){
   	Matrix* matrix = new Matrix(1, 1);
   	matrix->columns(2);
   	auto [x, y] = matrix->getShape();
   	
   	bool expected = (x != 1) && (y==2);
   	
   	REQUIRE(expected);
   }
}








