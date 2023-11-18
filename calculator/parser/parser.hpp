#pragma once

#include <iostream>
#include <vector>
class Parser{

#define NUMVARS 26

public:

    Parser();
    ~Parser();
    double eval_exp(char *exp);
    char errormsg[64];
    

protected:

    void eval_exp1(double &result);
    void eval_exp2(double &result);
    void eval_exp3(double &result);
    void eval_exp4(double &result);
    void eval_exp5(double &result);
    void eval_exp6(double &result);
    void get_token();

    char *exp_ptr; // points to the expression
    char token[256]; // holds current token
    char tok_type; // holds token's type
    double vars[NUMVARS]; // holds variable's values
};


std::ostream &operator<<(std::ostream &out, const Parser &value);