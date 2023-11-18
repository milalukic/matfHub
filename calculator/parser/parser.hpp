#pragma once

#include <iostream>
#include <vector>
class Parser{

public:

    Parser();
    ~Parser();
    double eval_exp(char *exp);
    char errormsg[64];

private:

    const int NUMVARS = 26;
    char *exp_ptr; // points to the expression
    char token[256]; // holds current token
    char tok_type; // holds token's type
    std::vector<double>(NUMVARS); // holds variable's values
};


