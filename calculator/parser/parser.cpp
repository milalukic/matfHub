#include "parser.hpp"

//constructors
    //constructor
Parser::Parser()
{
    int i;
    exp_ptr = NULL;
    for (i = 0; i < NUMVARS; i++)
        vars[i] = 0.0;
    errormsg[0] = '\0';
}
    //destructor
Parser::~Parser(){
    std::cout << "Unisten: parser" << std::endl;
}



