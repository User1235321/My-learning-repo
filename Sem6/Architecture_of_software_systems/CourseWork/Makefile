CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./src
SRC_DIR = src
TEST_DIR = test
BUILD_DIR = build
LIB = $(BUILD_DIR)/libos.a

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_BINS = $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%, $(TEST_SRCS))

.PHONY: build run clear test

build: $(LIB)

$(LIB): $(OBJS)
	mkdir -p $(BUILD_DIR)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/internal.h $(SRC_DIR)/rtos_api.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clear:
	rm -rf $(BUILD_DIR)

test: $(TEST_BINS)
	@for exe in $(TEST_BINS); do \
		echo "Running $$exe..."; \
		./$$exe || exit 1; \
	done

$(BUILD_DIR)/%: $(TEST_DIR)/%.c $(LIB)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -L$(BUILD_DIR) -los -o $@

run:
	@echo "No default program. Use 'make test' to run tests."
