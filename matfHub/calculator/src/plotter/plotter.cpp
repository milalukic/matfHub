#include "plotter.hpp"

//konstruktor
Plotter::Plotter(std::string func){
    this->_func = func;
}

//metode
void plot_sin(){
    matplot::fplot("sin(x)", "o-r")->line_width(2);
    matplot::show();
}