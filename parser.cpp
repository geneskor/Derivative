#include "parser.hpp"
#include <cctype>
#include <cmath>
#include <stdexcept>

// Конструктор, инициализирующий входную строку и начальную позицию
Parser::Parser(const std::string& input) : input(input), pos(0) {}

// Пропускает все пробелы в текущей позиции
void Parser::skip_whitespace() {
    while (pos < input.size() && std::isspace(input[pos])) {
        pos++;
    }
}

// Возвращает текущий символ без его потребления
char Parser::peek() {
    skip_whitespace();  // Пропускаем пробелы перед чтением
    return (pos < input.size()) ? input[pos] : '\0';  // Возвращаем текущий символ или нулевой символ, если строка закончилась
}

// Потребляет текущий символ и возвращает его
char Parser::consume() {
    skip_whitespace();  // Пропускаем пробелы перед чтением
    if (pos >= input.size()) {
        throw std::runtime_error("Unexpected end of input");  // Если строка закончилась, выбрасываем исключение
    }
    return input[pos++];  // Возвращаем текущий символ и увеличиваем позицию
}

// Проверяет, совпадает ли текущий символ с ожидаемым, и потребляет его, если да
bool Parser::match(char expected) {
    skip_whitespace();  // Пропускаем пробелы перед проверкой
    if (pos < input.size() && input[pos] == expected) {
        pos++;  // Потребляем символ, если он совпадает с ожидаемым
        return true;
    }
    return false;  // Возвращаем false, если символ не совпадает
}

// Основной метод, который запускает парсинг выражения
Expression<double> Parser::parse() {
    return parse_expression();  // Начинаем с парсинга выражения
}

// Парсит выражение, состоящее из термов, соединенных операциями сложения и вычитания
Expression<double> Parser::parse_expression() {
    Expression<double> left = parse_term();  // Парсим первый терм
    while (true) {
        if (match('+')) {
            left = left + parse_term();  // Если встретили +, добавляем следующий терм
        } else if (match('-')) {
            left = left - parse_term();  // Если встретили -, вычитаем следующий терм
        } else {
            break;  // Если больше нет операций сложения или вычитания, выходим из цикла
        }
    }
    return left;  // Возвращаем результат
}

// Парсит терм, состоящий из факторов, соединенных операциями умножения и деления
Expression<double> Parser::parse_term() {
    Expression<double> left = parse_factor();  // Парсим первый фактор
    while (true) {
        if (match('*')) {
            left = left * parse_factor();  // Если встретили *, умножаем на следующий фактор
        } else if (match('/')) {
            left = left / parse_factor();  // Если встретили /, делим на следующий фактор
        } else {
            break;  // Если больше нет операций умножения или деления, выходим из цикла
        }
    }
    return left;  // Возвращаем результат
}

// Парсит фактор, который может быть возведен в степень
Expression<double> Parser::parse_factor() {
    Expression<double> left = parse_primary();  // Парсим первичное выражение
    if (match('^')) {
        left = left ^ parse_factor();  // Если встретили ^, возводим в степень следующий фактор
    }
    return left;  // Возвращаем результат
}

// Парсит первичное выражение: число, переменную, функцию или выражение в скобках
Expression<double> Parser::parse_primary() {
    if (match('(')) {
        Expression<double> expr = parse_expression();  // Если встретили (, парсим выражение в скобках
        if (!match(')')) {
            throw std::runtime_error("Expected ')'");  // Если после выражения нет ), выбрасываем исключение
        }
        return expr;  // Возвращаем выражение в скобках
    } else if (std::isalpha(peek())) {
        // Если текущий символ - буква, это может быть функция или переменная
        char nextChar = peek();
        if (nextChar == 'x' || nextChar == 'y') {
            return parse_variable();  // Если это 'x' или 'y', парсим переменную
        } else {
            return parse_function();  // Иначе парсим функцию
        }
    } else if (std::isdigit(peek()) || peek() == '.') {
        return parse_number();  // Если это цифра или точка, парсим число
    } else {
        throw std::runtime_error("Unexpected character");  // Если символ не распознан, выбрасываем исключение
    }
}

// Парсит вызов функции (sin, cos, ln, exp)
Expression<double> Parser::parse_function() {
    std::string func;
    while (std::isalpha(peek())) {
        func += consume();  // Собираем имя функции
    }

    // В зависимости от имени функции, вызываем соответствующую операцию
    if (func == "sin") {
        return parse_primary().sin();
    } else if (func == "cos") {
        return parse_primary().cos();
    } else if (func == "ln") {
        return parse_primary().ln();
    } else if (func == "exp") {
        return parse_primary().exp();
    } else {
        throw std::runtime_error("Unknown function: " + func);  // Если функция неизвестна, выбрасываем исключение
    }
}

// Парсит число
Expression<double> Parser::parse_number() {
    std::string num;
    while (std::isdigit(peek()) || peek() == '.') {
        num += consume();  // Собираем цифры и точки в строку
    }
    return Expression<double>(std::stod(num));  // Преобразуем строку в число и возвращаем его
}

// Парсит переменную (x, y)
Expression<double> Parser::parse_variable() {
    std::string var;
    var += consume();  // Потребляем символ переменной
    return Expression<double>(var);  // Возвращаем переменную
}
