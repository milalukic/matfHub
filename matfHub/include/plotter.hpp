#ifndef PLOTTER_HPP

#define PLOTTER_HPP

#include <iostream>
#include <vector>
#include <armadillo>
#include <matplot/matplot.h>

class Plotter{

public:
    //konstruktor
    Plotter();
    //destruktor
    ~Plotter();

    //metode
    void plot_sin();

// private:
//     std::string _func;
};

#endif
