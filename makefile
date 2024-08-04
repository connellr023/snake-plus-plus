# Compiler and flags
CXX = g++
CXXFLAGS = -Wall

# Target executable name
TARGET = build

# Source files
SRCS = \
    main.cpp \
    framebuffer/linux_framebuffer.cpp \
    game/game.cpp \
    game/snake/snake.cpp \
    rendering/rendering.cpp \

# Object files
OBJS = $(SRCS:.cpp=.o)

# Libraries
LIBS = -lX11

# Default target
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Rule to build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the application
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean run
