#define CATCH_CONFIG_MAIN

#include "../catch.hpp"
#include "../include/statistics.hpp"


TEST_CASE("Statistics", "[class]"){

    SECTION("method mean returns -nan, if there are no values"){
        Statistics stat = Statistics();
        std::vector<double> input = std::vector<double>();
        tat.xData(input);

        auto output = stat.mean();

        REQUIRE(std::isnan(output));
    }

    SECTION("method mean returns the value, if there is one value in the dataset"){
        Statistics stat = Statistics();
        std::vector<double> input = {1};
        tat.xData(input);
        const double expectedOutput = 0;

        auto output = stat.mean();

        REQUIRE(expectedOutput == output);
    }

    SECTION("method mean returns the mean of the data, if the data is valid"){
        Statistics stat = Statistics();
        std::vector<double> input = {1, 2, 3};
        tat.xData(input);
        const double expectedOutput = 2;

        auto output = stat.mean();

        REQUIRE(expectedOutput == output);
    }

    SECTION("method variance returns 0, if there are no values"){
        Statistics stat = Statistics();
        std::vector<double> input = std::vector<double>();
        stat.xData(input);
        const double expectedOutput = 0;

        auto output = stat.variance();

        REQUIRE(expectedOutput == output);
    }

    SECTION("method variance returns nan, if there is one value in the dataset"){
        Statistics stat = Statistics();
        std::vector<double> input = {1};
        stat.xData(input);

        auto output = stat.mean();

        REQUIRE(std::isnan(output));
    }

    SECTION("method variance returns the variance of the data, if the data is valid"){
        Statistics stat = Statistics();
        std::vector<double> input = {1, 2, 3};
        const double expectedOutput = 2;
        stat.xData(input);

        auto output = stat.mean();

        REQUIRE(expectedOutput == output);
    }


    //TODO exception
    SECTION("method median throws an exception, if there are no values"){
        Statistics stat = Statistics();
        std::vector<double> input = std::vector<double>();
        const double expectedOutput = 0;

        auto output = stat.median();

        REQUIRE(expectedOutput == output);
    }

    SECTION("method median returns the value, if there is one value in the dataset"){
        Statistics stat = Statistics();
        std::vector<double> input = {1};
        const double expectedOutput = 0;

        auto output = stat.median();

        REQUIRE(expectedOutput == output);
    }

    SECTION("method median returns the mean of the middle two values of the data, if there is an even number of values in the dataset"){
        Statistics stat = Statistics();
        std::vector<double> input = {1, 2, 3, 4};
        const double expectedOutput = 2.5;

        auto output = stat.median();

        REQUIRE(expectedOutput == output);
    }

    SECTION("method median returns the median of the data, if there is an odd number of values in the dataset"){
        Statistics stat = Statistics();
        std::vector<double> input = {1, 2, 3, 4, 5};
        const double expectedOutput = 3;

        auto output = stat.median();

        REQUIRE(expectedOutput == output);
    }



    SECTION("method mode returns 0, if there are no values in the data"){
        Statistics stat = Statistics();
        std::vector<double> input = std::vector<double>();
        const double expectedOutput = 0;

        auto output = stat.mode();

        REQUIRE(expectedOutput == output);
    }

    SECTION("method mode returns the value, if there is one value in the data"){
        Statistics stat = Statistics();
        std::vector<double> input = {1};
        const double expectedOutput = 1;

        auto output = stat.mode();

        REQUIRE(expectedOutput == output);
    }

    SECTION("method mode returns the most frequent value, if the dataset is valid"){
        Statistics stat = Statistics();
        std::vector<double> input = {1, 2, 3, 1, 1};
        const double expectedOutput = 1;

        auto output = stat.mode();

        REQUIRE(expectedOutput == output);
    }

    SECTION("method mode returns the smallest most frequent value, if there are more values of the same frequency"){
        Statistics stat = Statistics();
        std::vector<double> input = {1, 2, 3, 2, 1};
        const double expectedOutput = 1;

        auto output = stat.mode();

        REQUIRE(expectedOutput == output);
    }
 
}