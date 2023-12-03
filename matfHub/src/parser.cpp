#include "../include/parser.hpp"
#include "../include/parserEnum.hpp"
#include <cstring>
#include <math.h> 

//TODO refactor code

//TODO fix
#define PI 3.14159265358979323846 

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

double Parser::eval_exp(char *exp)
{
    errormsg[0] = '\0';
    double result;
    exp_ptr = exp;
    get_token();
    if (!*token) 
    {
        strcpy(errormsg, "No Expression Present"); // no expression present
        return (double)0;
    }
    eval_exp1(result);
    if (*token) // last token must be null
        strcpy(errormsg, "Syntax Error");
    return result;
}

void Parser::eval_exp1(double &result)
{
    int slot;
    char temp_token[80];
    if (tok_type == VARIABLE) 
    {
        // save old token
        char *t_ptr = exp_ptr;
        strcpy(temp_token, token);
        // compute the index of the variable
        slot = *token - 'A';
        get_token();
        if (*token != '=') 
        {
            exp_ptr = t_ptr; // return current token
            strcpy(token, temp_token); // restore old token
            tok_type = VARIABLE;
        }
        else {
            get_token(); // get next part of exp
            eval_exp2(result);
            vars[slot] = result;
            return;
        }
    }
    eval_exp2(result);
}
// Add or subtract two terms.
void Parser::eval_exp2(double &result)
{
    char op;
    double temp;
    eval_exp3(result);
    while ((op = *token) == '+' || op == '-')
    {
        get_token();
        eval_exp3(temp);
        switch (op) 
        {
        case '-':
            result = result - temp;
            break;
        case '+':
            result = result + temp;
            break;
        }
    }
}
// Multiply or divide two factors.
void Parser::eval_exp3(double &result)
{
    char op;
    double temp;
    eval_exp4(result);
    while ((op = *token) == '*' || op == '/') 
    {
        get_token();
        eval_exp4(temp);
        switch (op) 
        {
        case '*':
            result = result * temp;
            break;
        case '/':
            result = result / temp;
            break;
        }
    }
}
// Process an exponent.
void Parser::eval_exp4(double &result)
{
    double temp;
    eval_exp5(result);
    while (*token == '^')
    {
        get_token();
        eval_exp5(temp);
        result = pow(result, temp);
    }
}
// Evaluate a unary + or -.
void Parser::eval_exp5(double &result)
{
    char op;
    op = 0;
    if ((tok_type == DELIMITER && *token == '+') || *token == '-')
    {
        op = *token;
        get_token();
    }
    eval_exp6(result);
    if (op == '-')
        result = -result;
}
// Process a function, a parenthesized expression, a value or a variable
void Parser::eval_exp6(double &result)
{
    bool isfunc = (tok_type == FUNCTION);
    char temp_token[80];
    if (isfunc)
    {
        strcpy(temp_token, token);
        get_token();
    } 
    if ((*token == '(')) 
    {
        get_token();
        eval_exp2(result);
        if (*token != ')')
            strcpy(errormsg, "Unbalanced Parentheses");
        if (isfunc)
        {
            if (!strcmp(temp_token, "SIN"))
                result = sin(PI / 180 * result);
            else if (!strcmp(temp_token, "COS"))
                result = cos(PI / 180 * result);
            else if (!strcmp(temp_token, "TAN"))
                result = tan(PI / 180 * result);
            else if (!strcmp(temp_token, "ASIN"))
                result = 180 / PI*asin(result);
            else if (!strcmp(temp_token, "ACOS"))
                result = 180 / PI*acos(result);
            else if (!strcmp(temp_token, "ATAN"))
                result = 180 / PI*atan(result);
            else if (!strcmp(temp_token, "SINH"))
                result = sinh(result);
            else if (!strcmp(temp_token, "COSH"))
                result = cosh(result);
            else if (!strcmp(temp_token, "TANH"))
                result = tanh(result);
            else if (!strcmp(temp_token, "ASINH"))
                result = asinh(result);
            else if (!strcmp(temp_token, "ACOSH"))
                result = acosh(result);
            else if (!strcmp(temp_token, "ATANH"))
                result = atanh(result);
            else if (!strcmp(temp_token, "LN"))
                result = log(result);
            else if (!strcmp(temp_token, "LOG"))
                result = log10(result);
            else if (!strcmp(temp_token, "EXP"))
                result = exp(result);
            else if (!strcmp(temp_token, "SQRT"))
                result = sqrt(result);
            else if (!strcmp(temp_token, "SQR"))
                result = result*result;
            else if (!strcmp(temp_token, "ROUND"))
                result = round(result);
            else if (!strcmp(temp_token, "INT"))
                result = floor(result);
            //TODO make it work
            // else if (!strcmp(temp_token, "PI"))
            //     result = PI;
            else
                strcpy(errormsg, "Unknown Function");
        }
        get_token();
    }
    else
        switch (tok_type)
        {
        case VARIABLE:
            result = vars[*token - 'A'];
            get_token();
            return;
        case NUMBER:
            result = atof(token);
            get_token();
            return;
        default:
            strcpy(errormsg, "Syntax Error");
        }
}

void Parser::get_token()
{
    char *temp;
    tok_type = 0;
    temp = token;
    *temp = '\0';
    if (!*exp_ptr)  // at end of expression
        return;
    while (isspace(*exp_ptr))  // skip over white space
        ++exp_ptr; 
    if (strchr("+-*/%^=()", *exp_ptr)) 
    {
        tok_type = DELIMITER;
        *temp++ = *exp_ptr++;  // advance to next char
    }
    else if (isalpha(*exp_ptr)) 
    {
        while (!strchr(" +-/*%^=()\t\r", *exp_ptr) && (*exp_ptr))
            *temp++ = toupper(*exp_ptr++);
        while (isspace(*exp_ptr))  // skip over white space
            ++exp_ptr;
        tok_type = (*exp_ptr == '(') ? FUNCTION : VARIABLE;
    }
    else if (isdigit(*exp_ptr) || *exp_ptr == '.')
    {
        while (!strchr(" +-/*%^=()\t\r", *exp_ptr) && (*exp_ptr))
            *temp++ = toupper(*exp_ptr++);
        tok_type = NUMBER;
    }
    *temp = '\0';
    if ((tok_type == VARIABLE) && (token[1]))
        strcpy(errormsg, "Only first letter of variables is considered");
}

void calculate(){

}

// std::ostream &operator<<(std::ostream &out, const Parser &value){
//     return << out << 
// }


