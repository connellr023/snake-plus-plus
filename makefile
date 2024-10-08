# Compiler and flags
CXX = g++
CXXFLAGS = -Wall

# Target executable name
TARGET = build

# Platform-specific settings
ifeq ($(OS),Windows_NT)
    # Windows settings
    RM = del
    SRCS = \
        src/main.cpp \
        src/rendering/rendering.cpp \
        src/framebuffer/windows/windows_framebuffer.cpp \
        src/game/game.cpp \
        src/game/snake/snake.cpp \
        src/game/ghost/ghost.cpp \
        src/start_screen/start_screen.cpp \
        src/over_screen/over_screen.cpp \
        src/time_manager/time_manager.cpp \

    # Static linking settings
    CXXFLAGS += -static
    LIBS = -lgdi32 -lmsvcrt
else
    # Linux settings
    RM = rm -f
    SRCS = \
        src/main.cpp \
        src/rendering/rendering.cpp \
        src/framebuffer/linux/linux_framebuffer.cpp \
        src/game/game.cpp \
        src/game/snake/snake.cpp \
        src/game/ghost/ghost.cpp \
        src/start_screen/start_screen.cpp \
        src/over_screen/over_screen.cpp \
        src/time_manager/time_manager.cpp \

    # Static linking settings
    CXXFLAGS += -static-libgcc -static-libstdc++
    LIBS = -lX11
endif

# Object files
OBJS = $(SRCS:.cpp=.o)

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
ifeq ($(OS),Windows_NT)
	del /F /Q $(subst /,\,$(OBJS)) $(TARGET).exe
else
	$(RM) $(OBJS) $(TARGET)
endif

# Run the application
run: $(TARGET)
	./$(TARGET)

# Release build with optimizations
release: CXXFLAGS += -O2
release: all

# Phony targets
.PHONY: all clean run release
