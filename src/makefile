CXX = clang++
CXXFLAGS = -I/opt/homebrew/Cellar/sfml/2.6.2/include
LDFLAGS = -L/opt/homebrew/Cellar/sfml/2.6.2/lib -lsfml-graphics -lsfml-window -lsfml-system

TARGET = main
SRC = main.cpp
OBJ = $(SRC:.cpp=.o)

build: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clear:
	rm -f $(TARGET) $(OBJ)

run:
	./main

all:
	make clear && make build && make run