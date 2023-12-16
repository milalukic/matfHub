#include "../include/calculator.h"

#include <QFile>
#include <fstream>

Calculator::Calculator(std::vector<std::string> h){}

Calculator::~Calculator(){
    std::cout << "Calculator unisten" << std::endl;
}

//TODO enkapsulacija? friendly?
void Calculator::writeHistory(std::string line){
    this->_history.push_back(line);
}

void Calculator::editLastLine(std::string line){
    auto history = this->history();
    int numOfLastLine = history.size()-1;
    history[numOfLastLine] = line;
    this->history(history);
}

std::string Calculator::lastLine(){
    auto history = this->history();
    int numOfLastLine = history.size()-1;
    return history[numOfLastLine];
}

void Calculator::saveHistory(){

    std::fstream file("history.txt");
    std::cout << "Hist" << std::endl;
    auto history = this->history();
    for(auto const &x : history){
        file << x;
        file << "\n";
    }
    file.close();
}

//
std::vector<std::string> Calculator::history() const{
    return this->_history;
}

void Calculator::history(std::vector<std::string> &newHistory){
    this->_history = newHistory;
}


