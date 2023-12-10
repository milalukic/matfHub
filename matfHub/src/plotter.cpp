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
void Plotter::linSpace(double lowerBound, double upperBound, size_t step){
    // Promenjena funkcija
    std::vector<double>x = matplot::linspace(lowerBound, upperBound, step);
    this->xData(x);
    this->yData(x);
}

void Plotter::transformSin(){
    //TODO privremeni vector?
    std::vector<double>yPrev = this->yData();
    std::vector<double>y = matplot::transform(yPrev, [](auto yPrev) {return sin(yPrev); });

    this->yData(y);
}

void Plotter::transformCos(){
    std::vector<double>yPrev = this->yData();
    std::vector<double>y = matplot::transform(yPrev, [](auto yPrev) {return cos(yPrev); });

    this->yData(y);
}

void Plotter::transformTan(){
    std::vector<double>yPrev = this->yData();
    std::vector<double>y = matplot::transform(yPrev, [](auto yPrev) {return tan(yPrev); });

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

