#define CATCH_CONFIG_MAIN

#include "../catch.hpp"
#include "math.h"
#include "../include/plotter.hpp"

TEST_CASE("Plotter", "[class]"){

    SECTION("method linspace changes xData to lowerBound, if there is one point"){
        Plotter *plt = Plotter::getPlotter();
        int lowerBound = 1;
        int upperBound = 10;
        int points = 1;
        int expectedOutput = lowerBound;
        int expectedSize = 1;

        plt->linSpace(lowerBound, upperBound, points);
        std::vector<double> output = plt->xData();

        CHECK(output.size() == expectedSize);
        REQUIRE(output[0] == expectedOutput);
    }

     SECTION("method linspace changes xData to an empty vector, if there are zero points"){
        Plotter *plt = Plotter::getPlotter();
        int lowerBound = 1;
        int upperBound = 10;
        int points = 0;
        int expectedSize = 0;

        plt->linSpace(lowerBound, upperBound, points);
        std::vector<double> output = plt->xData();

        REQUIRE(output.size() == expectedSize);
    }

    SECTION("method linspace changes xData to a linspace vector, if there are more than 1 points"){
        Plotter *plt = Plotter::getPlotter();
        int lowerBound = 1;
        int upperBound = 5;
        int points = 5;
        std::vector<double> expectedOutput = {1, 2, 3, 4, 5};

        plt->linSpace(lowerBound, upperBound, points);
        std::vector<double> output = plt->xData();

        REQUIRE(output == expectedOutput);
    }

    SECTION("method transformData maps yData to f(yData), if the function provided is valid"){
        Plotter *plt = Plotter::getPlotter();
        std::vector<double> y = {0};
        plt->yData(y);
        plt->transformData(tan);
        std::vector<double> output = plt->yData();

        REQUIRE(output[0] == 0);
    }

    SECTION("method transformData changes yData[i] to nan, if the function provided is sqrt and yData[i] is a negative number"){
        Plotter *plt = Plotter::getPlotter();
        std::vector<double> y = {-1};
        plt->yData(y);
        plt->transformData(sqrt);
        std::vector<double> output = plt->yData();

        REQUIRE(std::isnan(output[0]));
    }
}