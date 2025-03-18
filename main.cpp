#include "expression.hpp"
#include <iostream>
#include <map>
#include <cstring>

// Функция для разбора аргументов командной строки
void parse_arguments(int argc, char* argv[], std::string& expression, std::map<std::string, double>& variables, bool& eval_mode, bool& diff_mode, std::string& diff_by) {
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--eval") == 0) {
            eval_mode = true;
            expression = argv[++i];
        } else if (std::strcmp(argv[i], "--diff") == 0) {
            diff_mode = true;
            expression = argv[++i];
        } else if (std::strcmp(argv[i], "--by") == 0) {
            diff_by = argv[++i];
        } else if (std::strstr(argv[i], "=") != nullptr) {
            // Обработка переменных (например, x=10)
            char* name = strtok(argv[i], "=");
            char* value = strtok(nullptr, "=");
            variables[name] = std::stod(value);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: differentiator --eval <expression> [x=value y=value ...] OR differentiator --diff <expression> --by <variable>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string expression;
    std::map<std::string, double> variables;
    bool eval_mode = false;
    bool diff_mode = false;
    std::string diff_by;

    parse_arguments(argc, argv, expression, variables, eval_mode, diff_mode, diff_by);

    try {
        if (eval_mode) {
            // Вычисление выражения
            Expression<double> expr = Expression<double>::from_string(expression);
            double result = expr.eval(variables);
            std::cout << result << std::endl;
        } else if (diff_mode) {
            // Символьное дифференцирование
            Expression<double> expr = Expression<double>::from_string(expression);
            Expression<double> derivative = expr.diff(diff_by).simplify();
            std::cout << derivative.to_string() << std::endl;
        } else {
            std::cerr << "Invalid mode. Use --eval or --diff." << std::endl;
            return EXIT_FAILURE;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
