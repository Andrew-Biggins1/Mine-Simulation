# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -I/opt/homebrew/Cellar/sfml/2.6.2/include
LDFLAGS = -L/opt/homebrew/Cellar/sfml/2.6.2/lib -lsfml-graphics -lsfml-window -lsfml-system

# Target name
TARGET = mine
TARGET2 = test
TARGET3 = wave

# Source files
SRC = main.cpp
SRC2 = test.cpp
SRC3 = wave.cpp

# Default rule
all: $(TARGET) $(TARGET2) $(TARGET3)

# Compile and link
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

$(TARGET2): $(SRC2)
	$(CXX) $(CXXFLAGS) -o $(TARGET2) $(SRC2) $(LDFLAGS)

$(TARGET3): $(SRC3)
	$(CXX) $(CXXFLAGS) -o $(TARGET3) $(SRC3) $(LDFLAGS)

# Clean rule
clean:
	rm -f $(TARGET) $(TARGET2) $(TARGET3)
