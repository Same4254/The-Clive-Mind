SRC_DIR := ./src/
BIN_DIR := ./bin/

SOURCE_FILES := cpp
OBJECT_FILES := o

CC       := g++
CPPFLAGS := -std=c++17 -O3 -Wall -I include/

SOURCES := $(shell find $(SRC_DIR) -name "*.$(SOURCE_FILES)")
OBJECTS := $(patsubst $(SRC_DIR)%.$(SOURCE_FILES), $(BIN_DIR)%.$(OBJECT_FILES), $(SOURCES))

all: $(OBJECTS)

$(BIN_DIR)%$(OBJECT_FILES): $(SRC_DIR)%$(SOURCE_FILES)
	@mkdir -p $(@D)

	$(CC) $(CPPFLAGS) -c $< -o $@

.PHONY: mnist
mnist: $(OBJECTS)
	$(CC) $(CPPFLAGS) Experiments/Mnist/mnistTraining.cpp -o mnist-test $(OBJECTS)

.PHONY: clean
clean:
	rm -r -f $(BIN_DIR)*