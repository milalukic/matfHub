#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <iostream>
#include <vector>

class Calculator
{
public:
    Calculator(std::vector<std::string> h = std::vector<std::string>());
    ~Calculator();

    //
    void writeHistory(std::string line);
    void editLastLine(std::string line);
    std::string lastLine();
    //
    std::vector<std::string> history() const;

    void history(std::vector<std::string> &newHistory);

    void saveHistory();

private:
    std::vector<std::string> _history;

};


#endif // CALCULATOR_H
