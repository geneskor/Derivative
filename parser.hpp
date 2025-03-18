#ifndef PARSER_HPP
#define PARSER_HPP

#include "expression.hpp"
#include <string>
#include <map>
#include <memory>
#include <stdexcept>

class Parser {
public:
    // Конструктор, принимающий входную строку для парсинга
    Parser(const std::string& input);

    // Основной метод, который запускает парсинг выражения
    Expression<double> parse();

private:
    std::string input;  // Входная строка, которую нужно распарсить
    size_t pos;         // Текущая позиция в строке

    // Пропускает пробелы в входной строке
    void skip_whitespace();

    // Возвращает текущий символ без его потребления
    char peek();

    // Потребляет текущий символ и возвращает его
    char consume();

    // Проверяет, совпадает ли текущий символ с ожидаемым, и потребляет его, если да
    bool match(char expected);

    // Рекурсивные методы для парсинга различных частей выражения
    Expression<double> parse_expression();  // Парсит выражение (сложение и вычитание)
    Expression<double> parse_term();        // Парсит терм (умножение и деление)
    Expression<double> parse_factor();      // Парсит фактор (степень)
    Expression<double> parse_primary();     // Парсит первичное выражение (число, переменная, функция, скобки)
    Expression<double> parse_function();    // Парсит вызов функции (sin, cos, ln, exp)
    Expression<double> parse_number();     // Парсит число
    Expression<double> parse_variable();   // Парсит переменную (x, y)
};

#endif // PARSER_HPP
