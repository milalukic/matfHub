#include "../include/history.hpp"

#include <fstream>

History::History(std::vector<std::string> h = std::vector<std::string>()){}
History::~History(){
    std::cout << "History unisten" << std::endl;
}

    //TODO enkapsulacija? friendly?
void History::writeHistory(std::string line){
    this->_history.push_back(line);
}

std::string History::lastLine(){
    auto history = this->history();
    int numOfLastLine = history.size()-1;
    return history[numOfLastLine];
}

void History::editLastLine(std::string line){
    auto history = this->history();
    int numOfLastLine = history.size()-1;
    history[numOfLastLine] = line;
    this->history(history);
}

void History::saveHistory(){
    std::ofstream file("history.txt");
    std::cout << "Hist" << std::endl;
    auto history = this->history();
    for(auto const &x : history){
        file << x;
        file << "\n";
    }
    file.close();
}

    
    //

std::vector<std::string> History::history() const{
    return this->_history;
}

void History::history(std::vector<std::string> &newHistory){
    this->_history = newHistory;
}

