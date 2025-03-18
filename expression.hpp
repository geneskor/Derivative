#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <string>
#include <map>
#include <memory>
#include <stdexcept>
#include <complex>
#include <cmath>
#include <sstream>
#include <type_traits>

// Предварительное объявление класса Parser
class Parser;

// Шаблонный класс Expression, представляющий арифметическое выражение
template<typename T>
class Expression {
public:
    // Конструкторы
    Expression(std::string variable) : impl_(std::make_shared<Variable>(variable)) {}
    Expression(T value) : impl_(std::make_shared<Value>(value)) {}
    Expression(const Expression& other) : impl_(other.impl_) {} // Конструктор копирования
    Expression& operator=(const Expression& other) { // Оператор копирования
        if (this != &other) {
            impl_ = other.impl_;
        }
        return *this;
    }
    Expression(Expression&& other) noexcept : impl_(std::move(other.impl_)) {} // Конструктор перемещения
    Expression& operator=(Expression&& other) noexcept { // Оператор перемещения
        if (this != &other) {
            impl_ = std::move(other.impl_);
        }
        return *this;
    }

    // Метод для создания выражения из строки
    static Expression<double> from_string(const std::string& input);

    // Арифметические операции
    Expression operator+(const Expression& that) const {
        return Expression(std::make_shared<OperationAdd>(*this, that)); // Сложение
    }
    Expression& operator+=(const Expression& that) {
        *this = *this + that; // Сложение с присваиванием
        return *this;
    }
    Expression operator-(const Expression& that) const {
        return Expression(std::make_shared<OperationSub>(*this, that)); // Вычитание
    }
    Expression& operator-=(const Expression& that) {
        *this = *this - that; // Вычитание с присваиванием
        return *this;
    }
    Expression operator*(const Expression& that) const {
        return Expression(std::make_shared<OperationMul>(*this, that)); // Умножение
    }
    Expression& operator*=(const Expression& that) {
        *this = *this * that; // Умножение с присваиванием
        return *this;
    }
    Expression operator/(const Expression& that) const {
        return Expression(std::make_shared<OperationDiv>(*this, that)); // Деление
    }
    Expression& operator/=(const Expression& that) {
        *this = *this / that; // Деление с присваиванием
        return *this;
    }
    Expression operator^(const Expression& that) const {
        return Expression(std::make_shared<OperationPow>(*this, that)); // Возведение в степень
    }
    Expression& operator^=(const Expression& that) {
        *this = *this ^ that; // Возведение в степень с присваиванием
        return *this;
    }

    // Математические функции
    Expression sin() const {
        return Expression(std::make_shared<OperationSin>(*this)); // Синус
    }
    Expression cos() const {
        return Expression(std::make_shared<OperationCos>(*this)); // Косинус
    }
    Expression ln() const {
        return Expression(std::make_shared<OperationLn>(*this)); // Натуральный логарифм
    }
    Expression exp() const {
        return Expression(std::make_shared<OperationExp>(*this)); // Экспонента
    }

    // Вычисление выражения в заданном контексте (значения переменных)
    T eval(std::map<std::string, T> context) const {
        return impl_->eval(context); // Вызов eval у внутренней реализации
    }

    // Преобразование выражения в строку
    std::string to_string() const {
        return impl_->to_string(); // Вызов to_string у внутренней реализации
    }

    // Символьное дифференцирование
    Expression diff(const std::string& variable) const {
        return impl_->diff(variable); // Вызов diff у внутренней реализации
    }

    // Упрощение выражения
    Expression simplify() const {
        return impl_->simplify(); // Вызов simplify у внутренней реализации
    }

private:
    // Базовый класс для всех типов выражений (число, переменная, операции)
    class ExpressionImpl {
    public:
        virtual ~ExpressionImpl() = default;
        virtual T eval(std::map<std::string, T> context) const = 0; // Вычисление выражения
        virtual std::string to_string() const = 0; // Преобразование в строку
        virtual Expression diff(const std::string& variable) const = 0; // Символьное дифференцирование
        virtual Expression simplify() const = 0; // Упрощение выражения
    };

    // Класс, представляющий число
    class Value : public ExpressionImpl {
    public:
        Value(T value) : value_(value) {} // Конструктор для числа
        T eval(std::map<std::string, T> context) const override {
            (void)context; // Игнорируем контекст, так как это число
            return value_; // Возвращаем само число
        }
        std::string to_string() const override {
            std::ostringstream oss;
            if constexpr (std::is_same_v<T, std::complex<double>>) {
                oss << "(" << value_.real() << " + " << value_.imag() << "i)"; // Комплексное число
            } else {
                oss << value_; // Действительное число
            }
            return oss.str();
        }
        Expression diff(const std::string& variable) const override {
            return Expression(0.0); // Производная числа равна нулю
        }
        Expression simplify() const override {
            return Expression(value_); // Число уже упрощено
        }
    private:
        T value_; // Значение числа
    };

    // Класс, представляющий переменную
    class Variable : public ExpressionImpl {
    public:
        Variable(std::string name) : name_(name) {} // Конструктор для переменной
        T eval(std::map<std::string, T> context) const override {
            auto iter = context.find(name_); // Ищем переменную в контексте
            if (iter == context.end()) {
                throw std::runtime_error("Variable \"" + name_ + "\" not present in evaluation context"); // Ошибка, если переменная не найдена
            }
            return iter->second; // Возвращаем значение переменной
        }
        std::string to_string() const override {
            return name_; // Возвращаем имя переменной
        }
        Expression diff(const std::string& variable) const override {
            if (name_ == variable) {
                return Expression(1.0); // Производная по самой переменной равна 1
            } else {
                return Expression(0.0); // Производная по другой переменной равна 0
            }
        }
        Expression simplify() const override {
            return Expression(name_); // Переменная уже упрощена
        }
    private:
        std::string name_; // Имя переменной
    };

    // Класс, представляющий операцию сложения
    class OperationAdd : public ExpressionImpl {
    public:
        OperationAdd(Expression left, Expression right) : left_(left), right_(right) {} // Конструктор для сложения
        T eval(std::map<std::string, T> context) const override {
            return left_.eval(context) + right_.eval(context); // Складываем результаты левого и правого выражений
        }
        std::string to_string() const override {
            return "(" + left_.to_string() + " + " + right_.to_string() + ")"; // Возвращаем строку вида "(a + b)"
        }
        Expression diff(const std::string& variable) const override {
            return left_.diff(variable) + right_.diff(variable); // Производная суммы
        }
        Expression simplify() const override {
            Expression left = left_.simplify();
            Expression right = right_.simplify();

            // Если одно из слагаемых равно нулю, возвращаем другое
            if (left.to_string() == "0") {
                return right;
            }
            if (right.to_string() == "0") {
                return left;
            }

            // Иначе возвращаем упрощённое сложение
            return left + right;
        }
    private:
        Expression left_, right_; // Левый и правый операнды
    };

    // Класс, представляющий операцию умножения
    class OperationMul : public ExpressionImpl {
    public:
        OperationMul(Expression left, Expression right) : left_(left), right_(right) {} // Конструктор для умножения
        T eval(std::map<std::string, T> context) const override {
            return left_.eval(context) * right_.eval(context); // Умножаем результаты левого и правого выражений
        }
        std::string to_string() const override {
            return "(" + left_.to_string() + " * " + right_.to_string() + ")"; // Возвращаем строку вида "(a * b)"
        }
        Expression diff([[maybe_unused]] const std::string& variable) const override {
            return left_.diff(variable) * right_ + left_ * right_.diff(variable); // Правило произведения
        }
        Expression simplify() const override {
            Expression left = left_.simplify();
            Expression right = right_.simplify();

            // Если один из множителей равен нулю, возвращаем ноль
            if (left.to_string() == "0" || right.to_string() == "0") {
                return Expression(0.0);
            }

            // Если один из множителей равен единице, возвращаем другой
            if (left.to_string() == "1") {
                return right;
            }
            if (right.to_string() == "1") {
                return left;
            }

            // Иначе возвращаем упрощённое умножение
            return left * right;
        }
    private:
        Expression left_, right_; // Левый и правый операнды
    };

    // Класс, представляющий операцию вычитания
    class OperationSub : public ExpressionImpl {
    public:
        OperationSub(Expression left, Expression right) : left_(left), right_(right) {} // Конструктор для вычитания
        T eval(std::map<std::string, T> context) const override {
            return left_.eval(context) - right_.eval(context); // Вычитаем результаты левого и правого выражений
        }
        std::string to_string() const override {
            return "(" + left_.to_string() + " - " + right_.to_string() + ")"; // Возвращаем строку вида "(a - b)"
        }
        Expression diff(const std::string& variable) const override {
            return left_.diff(variable) - right_.diff(variable); // Производная разности
        }
        Expression simplify() const override {
            Expression left = left_.simplify();
            Expression right = right_.simplify();

            // Если вычитаемое равно нулю, возвращаем уменьшаемое
            if (right.to_string() == "0") {
                return left;
            }

            // Иначе возвращаем упрощённое вычитание
            return left - right;
        }
    private:
        Expression left_, right_; // Левый и правый операнды
    };

    // Класс, представляющий операцию деления
    class OperationDiv : public ExpressionImpl {
    public:
        OperationDiv(Expression left, Expression right) : left_(left), right_(right) {} // Конструктор для деления
        T eval(std::map<std::string, T> context) const override {
            T denominator = right_.eval(context); // Вычисляем знаменатель
            if (denominator == T(0)) {
                throw std::runtime_error("Division by zero"); // Ошибка при делении на ноль
            }
            return left_.eval(context) / denominator; // Делим результаты левого и правого выражений
        }
        std::string to_string() const override {
            return "(" + left_.to_string() + " / " + right_.to_string() + ")"; // Возвращаем строку вида "(a / b)"
        }
        Expression diff(const std::string& variable) const override {
            return (left_.diff(variable) * right_ - left_ * right_.diff(variable)) / (right_ * right_); // Производная частного
        }
        Expression simplify() const override {
            Expression left = left_.simplify();
            Expression right = right_.simplify();

            // Если числитель равен нулю, возвращаем ноль
            if (left.to_string() == "0") {
                return Expression(0.0);
            }

            // Если знаменатель равен единице, возвращаем числитель
            if (right.to_string() == "1") {
                return left;
            }

            // Иначе возвращаем упрощённое деление
            return left / right;
        }
    private:
        Expression left_, right_; // Левый и правый операнды
    };

    // Класс, представляющий операцию возведения в степень
    class OperationPow : public ExpressionImpl {
    public:
        OperationPow(Expression base, Expression exponent) : base_(base), exponent_(exponent) {}

        T eval(std::map<std::string, T> context) const override {
            return std::pow(base_.eval(context), exponent_.eval(context));
        }

        std::string to_string() const override {
            return "(" + base_.to_string() + " ^ " + exponent_.to_string() + ")";
        }

        Expression diff(const std::string& variable) const override {
            Expression base_derivative = base_.diff(variable);
            Expression exponent_derivative = exponent_.diff(variable);

            // Формула сложного дифференцирования: f(x)^g(x) * (g'(x) * ln(f(x)) + g(x) * f'(x) / f(x))
            Expression part1 = exponent_derivative * base_.ln();
            Expression part2 = exponent_ * (base_derivative / base_);
            Expression derivative = (base_ ^ exponent_) * (part1 + part2);

            return derivative;
        }

        Expression simplify() const override {
            Expression base = base_.simplify();
            Expression exponent = exponent_.simplify();

            if (exponent.to_string() == "0") {
                return Expression(1.0);
            }

            if (base.to_string() == "0") {
                return Expression(0.0);
            }

            return base ^ exponent;
        }

    private:
        Expression base_, exponent_;
    };

    // Класс, представляющий операцию синуса
    class OperationSin : public ExpressionImpl {
    public:
        OperationSin(Expression arg) : arg_(arg) {} // Конструктор для синуса
        T eval(std::map<std::string, T> context) const override {
            return std::sin(arg_.eval(context)); // Вычисляем синус
        }
        std::string to_string() const override {
            return "sin(" + arg_.to_string() + ")"; // Возвращаем строку вида "sin(a)"
        }
        Expression diff(const std::string& variable) const override {
            return arg_.cos() * arg_.diff(variable); // Производная синуса
        }
        Expression simplify() const override {
            return arg_.simplify().sin(); // Упрощаем аргумент и возвращаем синус
        }
    private:
        Expression arg_; // Аргумент синуса
    };

    // Класс, представляющий операцию косинуса
    class OperationCos : public ExpressionImpl {
    public:
        OperationCos(Expression arg) : arg_(arg) {} // Конструктор для косинуса
        T eval(std::map<std::string, T> context) const override {
            return std::cos(arg_.eval(context)); // Вычисляем косинус
        }
        std::string to_string() const override {
            return "cos(" + arg_.to_string() + ")"; // Возвращаем строку вида "cos(a)"
        }
        Expression diff(const std::string& variable) const override {
            return Expression(-1.0) * arg_.sin() * arg_.diff(variable); // Производная косинуса
        }
        Expression simplify() const override {
            return arg_.simplify().cos(); // Упрощаем аргумент и возвращаем косинус
        }
    private:
        Expression arg_; // Аргумент косинуса
    };

    // Класс, представляющий операцию натурального логарифма
    class OperationLn : public ExpressionImpl {
    public:
        OperationLn(Expression arg) : arg_(arg) {} // Конструктор для логарифма
        T eval(std::map<std::string, T> context) const override {
            return std::log(arg_.eval(context)); // Вычисляем логарифм
        }
        std::string to_string() const override {
            return "ln(" + arg_.to_string() + ")"; // Возвращаем строку вида "ln(a)"
        }
        Expression diff(const std::string& variable) const override {
            return (Expression(1.0) / arg_) * arg_.diff(variable); // Производная логарифма
        }
        Expression simplify() const override {
            return arg_.simplify().ln(); // Упрощаем аргумент и возвращаем логарифм
        }
    private:
        Expression arg_; // Аргумент логарифма
    };

    // Класс, представляющий операцию экспоненты
    class OperationExp : public ExpressionImpl {
    public:
        OperationExp(Expression arg) : arg_(arg) {} // Конструктор для экспоненты
        T eval(std::map<std::string, T> context) const override {
            return std::exp(arg_.eval(context)); // Вычисляем экспоненту
        }
        std::string to_string() const override {
            return "exp(" + arg_.to_string() + ")"; // Возвращаем строку вида "exp(a)"
        }
        Expression diff(const std::string& variable) const override {
            return arg_.exp() * arg_.diff(variable); // Производная экспоненты
        }
        Expression simplify() const override {
            return arg_.simplify().exp(); // Упрощаем аргумент и возвращаем экспоненту
        }
    private:
        Expression arg_; // Аргумент экспоненты
    };

    std::shared_ptr<ExpressionImpl> impl_; // Указатель на внутреннюю реализацию выражения

public:
    Expression(std::shared_ptr<ExpressionImpl> impl) : impl_(impl) {} // Приватный конструктор для внутреннего использования
};

// Объявления пользовательских литералов
Expression<double> operator"" _val(long double val);
Expression<double> operator"" _var(const char* variable, size_t size);
Expression<std::complex<double>> operator"" _val_c(long double val);
Expression<std::complex<double>> operator"" _var_c(const char* variable, size_t size);

#endif // EXPRESSION_HPP
