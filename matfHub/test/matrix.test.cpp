#define CATCH_CONFIG_MAIN


#include "../catch.hpp"
#include "../include/matrix.hpp"

TEST_CASE("Matrix calculator", "[class]"){

    SECTION("Method getShape returns dimensions of matrix"){
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

    SECTION("Methods rows and columns return dimensions of matrix, should be the same as getShape"){
    	Matrix* matrix = new Matrix(3, 5);
    	auto [x, y] = matrix->getShape();
    	
    	unsigned x1 = matrix->rows();
    	unsigned y1 = matrix->columns();
    	bool resultCorrect = (x == x1) && (y = y1);

    	REQUIRE(resultCorrect);

    }
   
}
