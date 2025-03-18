CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I.

SRCS = expression.cpp parser.cpp  main.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = differentiator

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
