#define CATCH_CONFIG_MAIN


#include "../catch.hpp"
#include "../include/parser.hpp"

TEST_CASE("Parser, regular expressions (not regex)", "[class]"){

    SECTION("method evalExpression returns nan, if the expression is invalid"){
        Parser parser = Parser();
        char* expression = "ad+wada*/ad";

        auto output = parser.evalExpression(expression);

        REQUIRE(std::isnan(output));
    }

    SECTION("method evalExpression returns the value of the first argument, if the addition expression is invalid"){
        Parser parser = Parser();
        char* expression = "5 +* 3";
        const double expectedOutput = 5;

        auto output = parser.evalExpression(expression);

        REQUIRE(expectedOutput == output);
    }

    SECTION("method evalExpression return the value of addition, if the expression of adition is valid"){
        Parser parser = Parser();
        char* expression = "5.3 + -3.3";
        const double expectedOutput = 2;

        auto output = parser.evalExpression(expression);

        REQUIRE(expectedOutput == output);
    }

    SECTION("method evalExpression returns the value of the first argument, if the multiplication expression is invalid"){
        Parser parser = Parser();
        char* expression = "5 +* 3";
        const double expectedOutput = 5;

        auto output = parser.evalExpression(expression);

        REQUIRE(expectedOutput == output);
    }

    SECTION("method evalExpression returns the result of multiplication, if the multiplication expression is valid"){
        Parser parser = Parser();
        char* expression = "2.5 * -2.5/1";
        const double expectedOutput = -6.25;

        auto output = parser.evalExpression(expression);

        REQUIRE(expectedOutput == output);
    }

    SECTION("method evalExpression returns the value of the argument, if the function doesn't exist"){
        Parser parser = Parser();
        char* expression = "dasd(4)";
        const double expectedOutput = 4;

        auto output = parser.evalExpression(expression);

        REQUIRE(expectedOutput == output);
    }

    SECTION("method evalExpression returns nan, if the function is not defined for that argument"){
        Parser parser = Parser();
        char* expression = "sqrt(-1)";

        auto output = parser.evalExpression(expression);

        REQUIRE(std::isnan(output));
    }

    SECTION("method evalExpression return the value of the function for that argument, if the function exists"){
        Parser parser = Parser();
        char* expression = "sqrt(4)";
        const double expectedOutput = 2;

        auto output = parser.evalExpression(expression);

        REQUIRE(expectedOutput == output);
    }

    //TODO fix
    SECTION("method evalExpression returns 0, if the parantheses are not paired correctly"){
        Parser parser = Parser();
        char* expression = ")5+3())";
        const double expectedOutput = 0;

        auto output = parser.evalExpression(expression);

        REQUIRE(expectedOutput == output);
    }

    SECTION("method evalExpression returns the value of the expression, if the parantheses are paired correctly"){
        Parser parser = Parser();
        char* expression = "(5+3)";
        const double expectedOutput = 8;

        auto output = parser.evalExpression(expression);

        REQUIRE(expectedOutput == output);
    }
}

TEST_CASE("Parser, variable initialization", "[class]"){
    SECTION("method evalExpression return 0, if the variable is only defined by it's name"){
        Parser parser = Parser();
        char* expression = "x";
        const double expectedOutput = 0;

        auto output = parser.evalExpression(expression);

        REQUIRE(expectedOutput == output);
    }

    SECTION("method evalExpression returns the value of the variable, if the variable is initialized correctly"){
        Parser parser = Parser();
        char* expression = "x=5";
        const double expectedOutput = 5;

        auto output = parser.evalExpression(expression);

        REQUIRE(expectedOutput == output);
    }

    SECTION("metohd evalExpression returns the value of the expression, if the variables are correctly initialized and the expression is valid"){
        Parser parser = Parser();
        char* tmp1 = "x=5";
        char* tmp2 = "y=6";
        char* expression = "x+y";
        const double expectedOutput = 11;

        auto expr1 = parser.evalExpression(tmp1);
        auto expr2 = parser.evalExpression(tmp2);
        auto output = parser.evalExpression(expression); 

        REQUIRE(expectedOutput == output);
    }
}