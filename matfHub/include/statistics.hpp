#ifndef STATISTICS_HPP

#define STATISTICS_HPP

#include <iostream>
#include <vector>

#include <cmath>
#include <map>
#include <numeric>

#include <matplot/matplot.h>

class Statistics{

public:
    //konstruktor
    Statistics(std::vector<double> const &xData = std::vector<double>());
    //destruktor
    ~Statistics();

    //metode
    double mean();
    double variance();
    double std();
    double median();
    double mode();
    
    void boxplot();
    void barplot();
    void histogram();

    
    //getters setters
    void xData(const std::vector<double> &newX);
    void textData(const std::vector<std::string> &newTextData);
    
    std::vector<double> xData() const;
    std::vector<std::string> textData() const;

private:

    std::vector<double> _xData;
    std::vector<std::string> _textData;
};

#endif // STATISTICS_HPP
