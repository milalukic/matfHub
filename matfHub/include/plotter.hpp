#ifndef PLOTTER_HPP

#define PLOTTER_HPP

#include <iostream>
#include <vector>

#include <cmath>

#include <matplot/matplot.h>

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

    void transformSin();
    void transformCos();
    void transformTan();
    void transformLn();
    void transformLog();
    void transformAbs();
    void transformNeg();
    void transformSquare();
    void transformRoot();
    void transformExp();

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
