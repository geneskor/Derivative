#include "expression.hpp"
#include <iostream>
#include <cstring>

int main(int argc, char* argv[])
{
    if (argc != 5 || std::strcmp(argv[1], "--x") != 0 || std::strcmp(argv[3], "--y") != 0)
    {
        printf("Usage: eval --x <x value> --y <y value>\n");
        return EXIT_FAILURE;
    }

    long double x = stold(std::string(argv[2]));
    long double y = stold(std::string(argv[4]));

    std::map<std::string, double> input_context =
    {
        {"x", static_cast<double>(x)}, {"y", static_cast<double>(y)}
    };

    Expression<double> expr1 = 100.0_val + "x"_var * 2.0_val;
    expr1 += "y"_var * "x"_var;

    std::map<std::string, double> context1 =
    {
        {"x", 1.0}, {"y", 2.0}
    };

    std::map<std::string, double> context2 =
    {
        {"x", 2.0}, {"y", 3.0}
    };

    printf("EVAL[%s]{x = %.1Lf, y = %.1Lf} = %Lf\n", expr1.to_string().c_str(), x, y, expr1.eval(input_context));

    printf("EVAL[%s]{x = 1.0, y = 2.0} = %Lf\n", expr1.to_string().c_str(), expr1.eval(context1));
    printf("EVAL[%s]{x = 2.0, y = 3.0} = %Lf\n", expr1.to_string().c_str(), expr1.eval(context2));

    return EXIT_SUCCESS;
}
