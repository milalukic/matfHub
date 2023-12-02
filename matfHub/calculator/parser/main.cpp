#include "parser.hpp"


int main()
{
    char expstr[256];
    Parser ob;
    std::cout << "Math expression parser. Enter a blank line to stop.\n\n";
    do
    {
        std::cout << "Enter expression: ";
        std::cin.getline(expstr, 255);
        double ans = ob.eval_exp(expstr);
        if (*ob.errormsg)
            std::cout << "Error: " << ob.errormsg << "\n\n";
        else
            std::cout << "Answer: " << ans << "\n\n";
    } while (*expstr);
    return 0;
}

