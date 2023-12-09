#include "../include/plotter.hpp"

//konstruktor
Plotter::Plotter(std::string func){
    this->_func = func;
}
Plotter::~Plotter(){
    std::cout << "Unisten plotter" << std::endl;
}

//metode
void Plotter::plotSin(std::vector<double>x){
    //TODO change to cpp algorithm?
    std::vector<double> y = matplot::transform(x, [](auto x) { return sin(x); });
    matplot::plot(x, y)->line_width(3).color("red");
    matplot::show();
}
//TODO change arguments to double
std::vector<double> Plotter::linSpace(double lowerBound, double upperBound, size_t step){
    return matplot::linspace(lowerBound, upperBound, step);
}

