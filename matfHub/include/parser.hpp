#pragma once

#include <iostream>
#include <vector>
#include <string.h>
class Parser{

#define NUMVARS 26

public:

    Parser();
    ~Parser();
    double eval_exp(char *exp);
    char errorMessage[64];
    

protected:

    void eval_exp1(double &result);
    void evalAdd(double &result);
    void evalMultiply(double &result);
    void evalPow(double &result);
    void evalUnar(double &result);
    void evalParantheses(double &result);
    void evalFunc(double &result, char* token);
    void getToken();

    void checkOperator(double &result, char op, double temp);


    char *expressionPtr; // points to the expression
    char token[256]; // holds current token
    char tokenType; // holds token's type
    double *vars; // holds variable's values


};


std::ostream &operator<<(std::ostream &out, const Parser &value);
