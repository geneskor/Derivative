#ifndef RUN_TESTS
#define RUN_TESTS

#include "expression.hpp"
#include "parser.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

// Тест для проверки сложения
void test_eval_addition() {
    Expression<double> expr = 5.0_val + 3.0_val;
    std::map<std::string, double> context;
    assert(expr.eval(context) == 8.0);
    std::cout << "test_eval_addition: OK\n";
}

// Тест для проверки вычитания
void test_eval_subtraction() {
    Expression<double> expr = 5.0_val - 3.0_val;
    std::map<std::string, double> context;
    assert(expr.eval(context) == 2.0);
    std::cout << "test_eval_subtraction: OK\n";
}

// Тест для проверки умножения
void test_eval_multiplication() {
    Expression<double> expr = 5.0_val * 3.0_val;
    std::map<std::string, double> context;
    assert(expr.eval(context) == 15.0);
    std::cout << "test_eval_multiplication: OK\n";
}

// Тест для проверки деления
void test_eval_division() {
    Expression<double> expr = 6.0_val / 3.0_val;
    std::map<std::string, double> context;
    assert(expr.eval(context) == 2.0);
    std::cout << "test_eval_division: OK\n";
}

// Тест для проверки функции синуса
void test_eval_sin() {
    Expression<double> expr = Expression<double>(0.0_val).sin();
    std::map<std::string, double> context;
    assert(expr.eval(context) == 0.0);
    std::cout << "test_eval_sin: OK\n";
}

// Тест для проверки функции косинуса
void test_eval_cos() {
    Expression<double> expr = Expression<double>(0.0_val).cos();
    std::map<std::string, double> context;
    assert(expr.eval(context) == 1.0);
    std::cout << "test_eval_cos: OK\n";
}

// Тест для проверки натурального логарифма
void test_eval_ln() {
    Expression<double> expr = Expression<double>(Expression<double>(1.0_val).exp()).ln();
    std::map<std::string, double> context;
    assert(std::abs(expr.eval(context) - 1.0) < 1e-9);
    std::cout << "test_eval_ln: OK\n";
}

// Тест для проверки экспоненты
void test_eval_exp() {
    Expression<double> expr = Expression<double>(1.0_val).exp();
    std::map<std::string, double> context;
    assert(std::abs(expr.eval(context) - std::exp(1.0)) < 1e-9);
    std::cout << "test_eval_exp: OK\n";
}

// Тест для проверки преобразования выражения в строку
void test_to_string() {
    Expression<double> expr = 5.0_val * 3.0_val;
    assert(expr.to_string() == "(5 * 3)");
    std::cout << "test_to_string: OK\n";
}

// Тест для проверки дифференцирования сложения
void test_differentiation_addition() {
    Expression<double> expr = "x"_var + 2.0_val;
    Expression<double> derivative = expr.diff("x");
    assert(derivative.to_string() == "(1 + 0)");
    std::cout << "test_differentiation_addition: OK\n";
}

// Тест для проверки парсинга выражения
void test_parse_expression() {
    Parser parser("3 + 5 * x");
    Expression<double> expr = parser.parse();
    assert(expr.to_string() == "(3 + (5 * x))");
    std::cout << "test_parse_expression: OK\n";
}

// Тест для проверки сложного дифференцирования (x^x)
void test_differentiation_power() {
    // Создаем выражение x^x
    Expression<double> expr = "x"_var ^ "x"_var;
    std::cout << "Expression: " << expr.to_string() << std::endl;  // Выводим исходное выражение

    // Вычисляем производную
    Expression<double> derivative = expr.diff("x");
    std::cout << "Derivative: " << derivative.to_string() << std::endl;  // Выводим производную

    // Проверяем корректность производной
    std::string expected_derivative = "((x ^ x) * ((1 * ln(x)) + (x * (1 / x))))";
    std::cout << "Expected derivative: " << expected_derivative << std::endl;  // Выводим ожидаемую производную

    // Проверяем, совпадает ли вычисленная производная с ожидаемой
    assert(derivative.to_string() == expected_derivative);
    std::cout << "test_differentiation_power: OK\n";
}

// Основная функция для запуска всех тестов
int main() {
    test_eval_addition();
    test_eval_subtraction();
    test_eval_multiplication();
    test_eval_division();
    test_eval_sin();
    test_eval_cos();
    test_eval_ln();
    test_eval_exp();
    test_to_string();
    test_differentiation_addition();
    test_parse_expression();
    test_differentiation_power();  // Добавленный тест с отладкой
    
    std::cout << "All tests passed successfully!\n";
    return 0;
}

#endif // RUN_TESTS
