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
    void plot();

    void linSpace(double lowerBound, double upperBound, size_t step);

    void transformSin();
    void transformCos();
    void transformTan();

    //TODO fill
    //getters setters
    void xData(std::vector<double> &newX);
    std::vector<double> xData() const;

    void yData(std::vector<double> &newY);
    std::vector<double> yData() const;


private:
    std::string _func;

    //TODO fix naming and comments
    std::vector<double> _xData; //linspace data
    std::vector<double> _yData;
};

#endif
