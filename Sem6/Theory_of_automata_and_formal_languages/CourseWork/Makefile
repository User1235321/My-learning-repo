CC   = g++
CFLAGS = -Wall -Wextra -Werror -O2
CC_VM = gcc
CFLAGS_VM = -Wall -O2

SRC_DIR = src
VM_DIR  = vm
TEST_DIR = test

TARGET     = cmilan
TARGET_VM  = mvm

all: $(TARGET) $(TARGET_VM)

$(TARGET): $(wildcard $(SRC_DIR)/*.cpp)
	$(CC) $(CFLAGS) $^ -o $@

$(TARGET_VM): $(wildcard $(VM_DIR)/*.c) $(wildcard $(VM_DIR)/*.h)
	$(CC_VM) $(CFLAGS_VM) $^ -o $@

run: $(TARGET) $(TARGET_VM)
	@if [ -z "$(FILE)" ]; then \
		echo "Usage: make run FILE=program.mil"; \
		exit 1; \
	fi
	@echo "Compiling $(FILE)..."
	./$(TARGET) $(FILE) > $(FILE).vm
	@echo "Running VM..."
	./$(TARGET_VM) $(FILE).vm

test: $(TARGET) $(TARGET_VM)
	@mkdir -p $(TEST_DIR)/output
	@for f in $(TEST_DIR)/*.mil; do \
		echo "=== $$f ==="; \
		./$(TARGET) $$f > $(TEST_DIR)/output/$$(basename $$f .mil).vm; \
		./$(TARGET_VM) $(TEST_DIR)/output/$$(basename $$f .mil).vm; \
		echo ""; \
	done

clean:
	rm -f $(TARGET) $(TARGET_VM)
	rm -rf $(TEST_DIR)/output

.PHONY: all run test clean
