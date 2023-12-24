#ifndef PLOTTER_HPP

#define PLOTTER_HPP

#include <iostream>
#include <vector>

#include <cmath>
#include <numeric>
#include <algorithm>
#include <matplot/matplot.h>

#include <string.h>

class Plotter{

public:
    //konstruktor
    Plotter(std::string func);
    //destruktor
    ~Plotter();

    //metode
    void plot();
    void operator()() const;

    void linSpace(double lowerBound, double upperBound, size_t step);
    void transformData(double (*func)(double));
    void transformDataParse(double (*func1)(double, char*), double (*func2)(double, char*));

    void savePlot();

    //TODO fill
    //getters setters
    void xData(std::vector<double> &newX);
    void yData(std::vector<double> &newY);
    void output(std::string text);

    std::vector<double> xData() const;
    std::vector<double> yData() const;
    std::string output() const;



private:
    std::string _func;

    //TODO fix naming and comments
    std::vector<double> _xData; //linspace data
    std::vector<double> _yData;

    std::string _output;
};

#endif
