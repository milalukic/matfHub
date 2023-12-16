#include "../include/plotter.hpp"


//konstruktor
Plotter::Plotter(std::string func){
    this->_func = func;
}
Plotter::~Plotter(){
    std::cout << "Unisten plotter" << std::endl;
}

void Plotter::operator()() const {
    matplot::plot(this->xData(), this->yData())->line_width(3).color("red");
    matplot::show();
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

void Plotter::transformLn(){
    std::vector<double>yPrev = this->yData();
    std::vector<double>y = matplot::transform(yPrev, [](auto yPrev) {return log(yPrev); });

    this->yData(y);
}

void Plotter::transformLog(){
    std::vector<double>yPrev = this->yData();
    std::vector<double>y = matplot::transform(yPrev, [](auto yPrev) {return log10(yPrev); });

    this->yData(y);
}

void Plotter::transformExp(){
    std::vector<double>yPrev = this->yData();
    std::vector<double>y = matplot::transform(yPrev, [](auto yPrev) {return exp(yPrev); });

    this->yData(y);
}

void Plotter::transformAbs(){
    std::vector<double>yPrev = this->yData();
    std::vector<double>y = matplot::transform(yPrev, [](auto yPrev) {return std::abs(yPrev); });

    this->yData(y);
}

void Plotter::transformNeg(){
    std::vector<double>yPrev = this->yData();
    std::vector<double>y = matplot::transform(yPrev, [](auto yPrev) {return -yPrev; });

    this->yData(y);
}

void Plotter::transformSquare(){
    std::vector<double>yPrev = this->yData();
    std::vector<double>y = matplot::transform(yPrev, [](auto yPrev) {return pow(yPrev, 2); });

    this->yData(y);
}

void Plotter::transformRoot(){
    std::vector<double>yPrev = this->yData();
    std::vector<double>y = matplot::transform(yPrev, [](auto yPrev) {return sqrt(yPrev); });

    this->yData(y);
}

//set
void Plotter::xData(std::vector<double> &newX)  {
    this->_xData = newX;
}

void Plotter::yData(std::vector<double> &newY)  {
    this->_yData = newY;
}

void Plotter::output(std::string text){
    this->_output = text;
}

//get
std::vector<double> Plotter::xData() const {
    return this->_xData;
}

std::vector<double> Plotter::yData() const {
    return this->_yData;
}

std::string Plotter::output() const{
    return this->_output;
}
