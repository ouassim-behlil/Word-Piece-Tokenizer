# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -I./include
DEBUGFLAGS = -g -fsanitize=address
OPTFLAGS = -O2

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# Files
SRCS = main.cpp $(SRC_DIR)/WordPieceTokenizer.cpp
OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
TARGET = tokenizer

# Default target
all: $(TARGET)

# Create object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)/$(SRC_DIR)

# Link
$(TARGET): $(OBJ_DIR) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)
	@echo "Build successful! Run with: ./$(TARGET)"

# Compile source files
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Debug build
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: clean $(TARGET)

# Optimized build
release: CXXFLAGS += $(OPTFLAGS)
release: clean $(TARGET)

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Clean everything including vocabulary
fclean: clean
	rm -f vocab.txt

# Rebuild
re: clean all

# Run the program
run: $(TARGET)
	./$(TARGET)

# Run with valgrind
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

.PHONY: all clean fclean re run debug release valgrind
