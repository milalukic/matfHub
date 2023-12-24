#include "../include/plotter.hpp"


//konstruktor
Plotter::Plotter(std::string func){
    this->_func = func;
}
Plotter::~Plotter(){
    std::cout << "Unisten plotter" << std::endl;
}


void Plotter::operator()() const {
    std::vector<double> zero(this->xData().size());
    std::fill(zero.begin(), zero.end(), 0);
    matplot::plot(zero, this->xData(), this->xData(), zero, this->xData(), this->yData());
    matplot::show();
}

//metode

void Plotter::linSpace(double lowerBound, double upperBound, size_t step){

    std::vector<double>x = matplot::linspace(lowerBound, upperBound, step);
    this->xData(x);
    this->yData(x);
}

//void Plotter::transformDataParse(double (*func1)(double, char*), double (*func2)(double, char*)){

//    std::vector<double>y = this->yData();
//    char* expr = "x=";
//    expr[2] = 5;
//    std::transform(begin(y), end(y), begin(y), [&func1, &func2](auto &x) {x = func1(x); return func2(x); });

//    this->yData(y);
//}

void Plotter::transformData(double (*func)(double)){

    std::vector<double>y = this->yData();
    std::transform(begin(y), end(y), begin(y), [&func](auto &x) {return func(x); });

    this->yData(y);
}

void Plotter::savePlot(){

    (*this)();
    matplot::save("../img/chart.jpg");
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
