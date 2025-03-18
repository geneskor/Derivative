#include "expression.hpp"
#include "parser.hpp"

// Реализация метода from_string
template<>
Expression<double> Expression<double>::from_string(const std::string& input) {
    Parser parser(input);
    return parser.parse();
}

// Определения пользовательских литералов
Expression<double> operator"" _val(long double val) {
    return Expression<double>(static_cast<double>(val));
}

Expression<double> operator"" _var(const char* variable, size_t size) {
    (void)size;
    return Expression<double>(std::string(variable));
}

Expression<std::complex<double>> operator"" _val_c(long double val) {
    return Expression<std::complex<double>>(static_cast<std::complex<double>>(val));
}

Expression<std::complex<double>> operator"" _var_c(const char* variable, size_t size) {
    (void)size;
    return Expression<std::complex<double>>(std::string(variable));
}
