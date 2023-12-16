#include "../include/history.hpp"

#include <fstream>
#include <QFile>

//TODO initialize?
History::History(std::vector<std::string>h){}
History::~History(){
    std::cout << "History unisten" << std::endl;
}

    //TODO enkapsulacija? friendly?
void History::writeHistory(std::string line){
    line += "\n";
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
    QFile file("../hist.txt");
    file.open(QIODevice::WriteOnly);
    auto history = this->history();
    for(auto const &x : history){
        file.write(x.c_str());
    }
}

std::vector<std::string> History::history() const{
    return this->_history;
}

void History::history(std::vector<std::string> &newHistory){
    this->_history = newHistory;
}

