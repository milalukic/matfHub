#ifndef HISTORY_H
#define HISTORY_H

#include <iostream>
#include <vector>

class History
{
public:
    History(std::vector<std::string> h = {" "});
    ~History();

    //
    void writeHistory(std::string title, std::string line);
    std::string lastLine();
    void editLastLine(std::string line);
    
    void saveHistory();

    //
    std::vector<std::string> history() const;

    void history(std::vector<std::string> &newHistory);

    //static?
    const std::string EOC = "--------------------\n";

private:
    std::vector<std::string> _history;

};


#endif // HISTORY_H
