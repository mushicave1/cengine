# Compiler and flags
CC = cc
BUILD_DIR = build
CFLAGS = -Wall -Wextra -std=c99 -g -O1 -fsanitize=address 

# User Args
GPU_BACKEND = opengl
BUILD_MODE = release

# CENGINE Internal Sources
SRC_DIR = src

SRC_CORE_DIR = $(SRC_DIR)/core
SRC_CORE_FILES = $(wildcard $(SRC_CORE_DIR)/*.c)
SRC_CORE_OBJ = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SRC_CORE_FILES:.c=.o))

SRC_GPU_DIR = $(SRC_DIR)/gpu
SRC_GPU_RENDERER = $(SRC_GPU_DIR)/gl_renderer.c
ifeq ($(GPU_BACKEND), vulkan)
SRC_GPU_RENDERER = $(SRC_GPU_DIR)/vk_renderer.c
CFLAGS += -DCENGINE_GPU_BACKEND_VULKAN
else
SRC_GPU_RENDERER = $(SRC_GPU_DIR)/gl_renderer.c
CFLAGS += -DCENGINE_GPU_BACKEND_OPENGL
endif
SRC_GPU_FILES = $(SRC_GPU_RENDERER)
SRC_GPU_OBJ = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SRC_GPU_FILES:.c=.o))

SRC_WINDOW_DIR = $(SRC_DIR)/window
SRC_WINDOW_FILES = $(wildcard $(SRC_WINDOW_DIR)/*.c)
SRC_WINDOW_OBJ = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SRC_WINDOW_FILES:.c=.o))

SRC_IMAGE_LOADER_DIR = $(SRC_DIR)/image_loader
SRC_IMAGE_LOADER_FILES = $(wildcard $(SRC_IMAGE_LOADER_DIR)/*.c)
SRC_IMAGE_LOADER_OBJ = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SRC_IMAGE_LOADER_FILES:.c=.o))


# CENGINE Internal Include
INCLUDE_DIR = include

# CENGINE External Sources
EXTERNAL_DIR = extern
EXTERNAL_GLAD_DIR = $(EXTERNAL_DIR)/glad
EXTERNAL_GLAD_FILES = $(wildcard $(EXTERNAL_GLAD_DIR)/src/*.c)
EXTERNAL_GLAD_OBJ = $(patsubst $(EXTERNAL_DIR)/%, $(BUILD_DIR)/%, $(EXTERNAL_GLAD_FILES:.c=.o))

# CENGINE Dependancies
EXTERNAL_GLFW_DIR = $(EXTERNAL_DIR)/glfw/3.4
EXTERNAL_GLFW_INCLUDE = $(EXTERNAL_GLFW_DIR)/include
EXTERNAL_GLFW_LIB = $(EXTERNAL_GLFW_DIR)/lib

EXTERNAL_STB_IMAGE_DIR = $(EXTERNAL_DIR)

# CENGINE Flags
FLAGS = -I$(INCLUDE_DIR) -I$(SRC_DIR) -I$(EXTERNAL_GLFW_INCLUDE) -I$(EXTERNAL_GLAD_DIR)/include -I$(EXTERNAL_STB_IMAGE_DIR)

# CENGINE Library
LIB = $(BUILD_DIR)/libcengine.a

# CENGINE Tests
TEST_DIR = test
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ = $(patsubst $(TEST_DIR)/%, $(BUILD_DIR)/$(TEST_DIR)/%, $(TEST_FILES:.c=.o))
TEST_EXE = $(BUILD_DIR)/test/tests

# CENGINE Examples
EXAMPLES_DIR = examples
EXAMPLES_BASIC_DIR = $(EXAMPLES_DIR)/basic
EXAMPLES_BASIC_FILES = $(EXAMPLES_BASIC_DIR)/basic.c
EXMAPLES_BASIC_OBJ = $(patsubst $(EXAMPLES_BASIC_DIR)/%, $(BUILD_DIR)/$(EXAMPLES_BASIC_DIR)/%, $(EXAMPLES_BASIC_FILES:.c=.o))
EXAMPLES_BASIC_EXE = $(BUILD_DIR)/examples/basic/basic_example


all: $(LIB)

tests: $(TEST_EXE)
	@echo "Running cengine tests..."
	./$<

examples-basic: $(EXAMPLES_BASIC_EXE)
	@echo "Running cenigne example $<"
	./$<


$(LIB): $(SRC_CORE_OBJ) $(SRC_GPU_OBJ) $(SRC_WINDOW_OBJ) $(SRC_IMAGE_LOADER_OBJ) $(EXTERNAL_GLAD_OBJ)
	@echo "Linking cengine objects into static library $@..."
	ar rcs $@ $^

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/core:
	mkdir -p $@

$(BUILD_DIR)/gpu:
	mkdir -p $@

$(BUILD_DIR)/window:
	mkdir -p $@

$(BUILD_DIR)/image_loader:
	mkdir -p $@

$(BUILD_DIR)/test:
	mkdir -p $@

$(BUILD_DIR)/glad/src:
	mkdir -p $@

$(BUILD_DIR)/examples/basic:
	mkdir -p $@

$(BUILD_DIR)/core/%.o: $(SRC_CORE_DIR)/%.c| $(BUILD_DIR) $(BUILD_DIR)/core
	@echo "Building cengine obj $@ at from source $<"
	$(CC) $(CFLAGS) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/gpu/%.o: $(SRC_GPU_DIR)/%.c| $(BUILD_DIR) $(BUILD_DIR)/gpu 
	@echo "Building cengine obj $@ at from source $<"
	$(CC) $(CFLAGS) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/window/%.o: $(SRC_WINDOW_DIR)/%.c| $(BUILD_DIR) $(BUILD_DIR)/window
	@echo "Building cengine obj $@ from source $<"
	$(CC) $(CFLAGS) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/image_loader/%.o: $(SRC_IMAGE_LOADER_DIR)/%.c| $(BUILD_DIR) $(BUILD_DIR)/image_loader
	@echo "Building cengine obj $@ from source $<"
	$(CC) $(CFLAGS) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/glad/src/%.o: $(EXTERNAL_GLAD_DIR)/src/%.c| $(BUILD_DIR) $(BUILD_DIR)/glad/src
	@echo "Building cengine obj $@ from source $<"
	$(CC) $(CFLAGS) $(FLAGS) -c $< -o $@

$(TEST_EXE): $(TEST_OBJ) $(LIB)
	@echo "Linking cengine tests into exe..."
	$(CC) $(CFLAGS) $(TEST_OBJ) -I$(TEST_DIR) $(LIB) -L$(BUILD_DIR) -lcengine -o $@

$(BUILD_DIR)/test/%.o: $(TEST_DIR)/%.c | $(BUILD_DIR) $(BUILD_DIR)/test
	@echo "Building cengine test: $<"
	$(CC) $(CFLAGS) $(FLAGS) -I$(TEST_DIR) -L$(BUILD_DIR) -lcengine -c $< -o $@

$(EXAMPLES_BASIC_EXE): $(EXMAPLES_BASIC_OBJ) $(LIB)
	@echo "Linking cengine example $< into exe $@..."
	$(CC) $(CFLAGS) $(EXMAPLES_BASIC_OBJ) -L$(BUILD_DIR) -lcengine -L$(EXTERNAL_GLFW_LIB) -lglfw3 -framework CoreFoundation -framework CoreGraphics -framework IOKit -framework Cocoa -o $@

$(BUILD_DIR)/examples/basic/%.o: $(EXAMPLES_BASIC_DIR)/%.c | $(BUILD_DIR) $(BUILD_DIR)/examples/basic
	$(CC) $(CFLAGS) $(FLAGS) -L$(BUILD_DIR) -lcengine -c $< -o $@

clean: $(BUILD_DIR)
	@echo "Removing build directory..."
	rm -rf $<

.PHONY: all clean






























# # Makefile for building cengine library and tests with glfw and glad

# # Compiler and flags
# CC = cc
# CFLAGS = -Wall -Wextra -std=c99 -g -I./src -I./include -I./extern/glfw/3.4/include -I./extern/glad/include
# ASMFLAGS = 


# # Directories
# BUILD_DIR = ./build
# SRC_DIR = ./src
# TEST_DIR = ./test
# EXTERN_DIR = ./extern
# INCLUDE_DIR = ./include

# # File lists
# CORE_SRC = $(wildcard $(SRC_DIR)/core/*.c)
# GPU_SRC = $(wildcard $(SRC_DIR)/gpu/*.c)
# WINDOW_SRC = $(wildcard $(SRC_DIR)/window/*.c)
# GLAD_SRC = $(EXTERN_DIR)/glad/src/glad.c
# ALL_SRC = $(CORE_SRC) $(GPU_SRC) $(WINDOW_SRC)
# ALL_INTERNAL_OBJ = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(ALL_SRC:.c=.o))
# GLAD_OBJ = $(patsubst $(EXTERN_DIR)/%, $(BUILD_DIR)/%, $(GLAD_SRC:.c=.o))
# ALL_INTERNAL_ASM = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(ALL_SRC:.c=.s))
# GLAD_ASM = $(patsubst $(EXTERN_DIR)/%, $(BUILD_DIR)/%, $(GLAD_SRC:.c=.s))


# # Include files
# CENGINE_H = $(INCLUDE_DIR)/cengine/cengine.h

# # Library output
# CENGINE_LIB = $(BUILD_DIR)/libcengine.a

# # Test executables
# TEST_SRC = $(wildcard $(TEST_DIR)/*.c)
# TEST_OBJ = $(patsubst $(TEST_DIR)/%, $(BUILD_DIR)/test/%, $(TEST_SRC:.c=.o))
# TEST_EXE = $(BUILD_DIR)/test/tests

# # Compile Commadns for clagnd intellisense


# # Default target
# all: $(CENGINE_LIB)

# tests: $(TEST_EXE)
# 	@echo "Running cengine tests..."
# 	./build/test/tests



# # Create the build directory if it doesn't exist
# $(BUILD_DIR):
# 	mkdir -p $(BUILD_DIR)

# # Ensure subdirectories for object files exist
# $(BUILD_DIR)/core:
# 	mkdir -p $(BUILD_DIR)/core

# $(BUILD_DIR)/gpu:
# 	mkdir -p $(BUILD_DIR)/gpu

# $(BUILD_DIR)/window:
# 	mkdir -p $(BUILD_DIR)/window

# $(BUILD_DIR)/glad/src:
# 	mkdir -p $(BUILD_DIR)/glad/src

# # Build the cengine static library
# $(CENGINE_LIB): $(ALL_INTERNAL_OBJ) $(GLAD_OBJ)
# 	ar rcs $@ $^

# # Compile C files into object files
# $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR) $(BUILD_DIR)/core $(BUILD_DIR)/gpu $(BUILD_DIR)/window
# 	@echo "Building $< at $@"
# 	$(CC) $(CFLAGS) -c $< -o $@

# # Compile glad into object file
# $(BUILD_DIR)/%.o: $(EXTERN_DIR)/%.c | $(BUILD_DIR) $(BUILD_DIR)/glad/src
# 	@echo "Building $< at $@"
# 	$(CC) $(CFLAGS) -c $< -o $@

# # Generate assembly files
# $(BUILD_DIR)/%.s: $(SRC_DIR)/%.c | $(BUILD_DIR) $(BUILD_DIR)/core $(BUILD_DIR)/gpu $(BUILD_DIR)/window
# 	$(CC) $(ASMFLAGS) $(CFLAGS) -S $< -o $@

# # Generate assembly files for glad
# $(BUILD_DIR)/%.s: $(EXTERN_DIR)/%.c | $(BUILD_DIR) $(BUILD_DIR)/glad/src
# 	@echo "Generating assembly for glad.c at $@"
# 	$(CC) $(ASMFLAGS) $(CFLAGS) -S $< -o $@

# # Build the single test executable (link all test objects)
# $(TEST_EXE): $(TEST_OBJ) $(CENGINE_LIB) | $(BUILD_DIR)/test
# 	@echo "Linking cengine tests into exe..."
# 	$(CC) $(CFLAGS) $(TEST_OBJ) -I$(TEST_DIR) $(CENGINE_LIB) -L$(BUILD_DIR) -lcengine -o $@

# # Compile test C files into object files
# $(BUILD_DIR)/test/%.o: $(TEST_DIR)/%.c | $(BUILD_DIR)/test
# 	@echo "Building cengine test: $<"
# 	$(CC) $(CFLAGS) -I$(TEST_DIR) -c $< -o $@

# $(BUILD_DIR)/test:
# 	mkdir -p $@

# # Clean up build artifacts
# clean:
# 	rm -rf $(BUILD_DIR)

# # Remove only object and library files
# clean-obj:
# 	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*.s $(BUILD_DIR)/libcengine.a

# # Remove test executables
# clean-tests:
# 	rm -f $(TEST_EXE)

# # PHONY targets
# .PHONY: all clean clean-obj clean-tests
