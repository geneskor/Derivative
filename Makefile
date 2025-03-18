CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I.

# Основная программа
SRCS = expression.cpp parser.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = differentiator

# Тесты
TEST_SRCS = test.cpp expression.cpp parser.cpp
TEST_OBJS = $(TEST_SRCS:.cpp=.o)
TEST_TARGET = test

# Сборка основной программы
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Сборка тестов (добавлен -DRUN_TESTS)
test: $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -DRUN_TESTS -o $(TEST_TARGET) $(TEST_OBJS)

# Компиляция отдельных файлов
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка
clean:
	rm -f $(OBJS) $(TARGET) $(TEST_OBJS) $(TEST_TARGET)

.PHONY: all clean test
