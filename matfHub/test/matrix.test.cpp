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
   
}
