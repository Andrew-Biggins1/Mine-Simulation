# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -I/opt/homebrew/Cellar/sfml/2.6.2/include
LDFLAGS = -L/opt/homebrew/Cellar/sfml/2.6.2/lib -lsfml-graphics -lsfml-window -lsfml-system

# Target name
TARGET = mine

# Source files
SRC = main.cpp

# Default rule
all: $(TARGET) 

# Compile and link
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

# Clean rule
clean:
	rm -f $(TARGET)
