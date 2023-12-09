#ifndef PLOTTER_HPP

#define PLOTTER_HPP

#include <iostream>
#include <vector>

#include <cmath>

#include <armadillo>
#include <matplot/matplot.h>

class Plotter{

public:
    //konstruktor
    Plotter(std::string func);
    //destruktor
    ~Plotter();

    //metode
    void plotSin(std::vector<double>x);

    std::vector<double> linSpace(double lowerBound, double upperBound, size_t step);

private:
    std::string _func;
};

#endif
