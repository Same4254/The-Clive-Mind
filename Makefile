SRC_DIR := ./src/
BIN_DIR := ./bin/
HEADER_DIR := ./include/

SOURCE_FILES := cpp
HEADER_FILES := hpp
OBJECT_FILES := o

CC       := g++
CPPFLAGS := -std=c++17 -O3 -Wall -I $(HEADER_DIR)

SOURCES := $(shell find $(SRC_DIR) -name "*.$(SOURCE_FILES)")
HEADERS := $(shell find $(HEADER_DIR) -name "*.$(HEADER_FILES)")
OBJECTS := $(patsubst $(SRC_DIR)%.$(SOURCE_FILES), $(BIN_DIR)%.$(OBJECT_FILES), $(SOURCES))

all: $(OBJECTS) $(HEADERS)

$(BIN_DIR)%$(OBJECT_FILES): $(SRC_DIR)%$(SOURCE_FILES) $(HEADERS)
	@mkdir -p $(@D)

	$(CC) $(CPPFLAGS) -c $< -o $@

.PHONY: mnist
mnist: $(OBJECTS) $(HEADERS)
	$(CC) $(CPPFLAGS) Experiments/Mnist/mnistTraining.cpp -o mnist-test $(OBJECTS)

.PHONY: test
test: $(OBJECTS) $(HEADERS)
	$(CC) $(CPPFLAGS) Tests/FileTest.cpp -o file-test $(OBJECTS)

.PHONY: clean
clean:
	rm -r -f $(BIN_DIR)*