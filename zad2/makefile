TARGET = main

CXX = g++
CXXFLAGS = -I /usr/include/eigen3 -std=c++17 -Wall -O2

SRC = main.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

tar:
	tar -czvf $(TARGET).tar.gz $(SRC) Makefile README.md
