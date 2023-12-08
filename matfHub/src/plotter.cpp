#include "../include/plotter.hpp"

//konstruktor
Plotter::Plotter(){
    // this->_func = func;
};

//metode
void Plotter::plot_sin(){
    matplot::fplot("sin(x)", "o-r");
    matplot::show();
}
