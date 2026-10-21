# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -I../core -I.

# Define the default version (can be overridden from the command line)
VERSION ?= "0.0.0"

# Pass VERSION as a string literal to CFLAGS
CFLAGS += -DVERSION=\"$(VERSION)\"

# Output binary name
TARGET := forge-cc 

# Directories
CORE_DIR := ../core
SRC_DIR := .

# Source files
# All .c files in ../core
CORE_SRCS := $(wildcard $(CORE_DIR)/*.c)
# All .c files in the current directory except main.c
LOCAL_SRCS := $(filter-out $(SRC_DIR)/main.c ./main.c main.c, $(wildcard $(SRC_DIR)/*.c))
MAIN_SRC := main.c

# Object files
CORE_OBJS := $(CORE_SRCS:.c=.o)
LOCAL_OBJS := $(LOCAL_SRCS:.c=.o)
MAIN_OBJ := $(MAIN_SRC:.c=.o)

# All objects required to link the final executable
ALL_OBJS := $(CORE_OBJS) $(LOCAL_OBJS) $(MAIN_OBJ)

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(CORE_OBJS) $(LOCAL_OBJS) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Rule for building object files in ../core
$(CORE_DIR)/%.o: $(CORE_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for building object files in current directory
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(TARGET) $(ALL_OBJS)

.PHONY: all clean
