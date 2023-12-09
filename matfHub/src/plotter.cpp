#include "../include/plotter.hpp"


//konstruktor
Plotter::Plotter(std::string func){
    this->_func = func;
}
Plotter::~Plotter(){
    std::cout << "Unisten plotter" << std::endl;
}

//metode
void Plotter::plot(){

    matplot::plot(this->xData(), this->yData())->line_width(3).color("red");
    matplot::show();
}

//TODO change arguments to double
std::vector<double> Plotter::linSpace(double lowerBound, double upperBound, size_t step){

    //TODO maknuti ? //POPRAVITI
    std::vector<double>x = this->xData();
    this->yData(x);

    return matplot::linspace(lowerBound, upperBound, step);
}

void Plotter::transformSin(){
    //TODO privremeni vector?
    std::vector<double>x = this->xData();
    std::vector<double>y = matplot::transform(x, [](auto x) {return sin(x); });

    this->yData(y);
}


void Plotter::xData(std::vector<double> &newX)  {
    this->_xData = newX;
}

//TODO auto preporuka?
std::vector<double> Plotter::xData() const {
    return this->_xData;
}


void Plotter::yData(std::vector<double> &newY)  {
    this->_yData = newY;
}

//TODO auto preporuka?
std::vector<double> Plotter::yData() const {
    return this->_yData;
}

