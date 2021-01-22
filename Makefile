SRC_DIR := ./src/
BIN_DIR := ./bin/
HEADER_DIR := ./include/
BUILD_DIR := ./Build
MAIN_DIR := ./Mains

SOURCE_FILES := cpp
HEADER_FILES := hpp
OBJECT_FILES := o

SOCKET_OBJECTS := ./SocketIOcpp/

CC       := g++
MPI      := mpic++
CPPFLAGS := -std=c++17 -g -Wall -I $(HEADER_DIR)
LDFLAGS  := -lpthread

SOURCES := $(shell find $(SRC_DIR) -name "*.$(SOURCE_FILES)")
HEADERS := $(shell find $(HEADER_DIR) -name "*.$(HEADER_FILES)")
OBJECTS := $(patsubst $(SRC_DIR)%.$(SOURCE_FILES), $(BIN_DIR)%.$(OBJECT_FILES), $(SOURCES))

all: $(OBJECTS) $(HEADERS)

$(BIN_DIR)%$(OBJECT_FILES): $(SRC_DIR)%$(SOURCE_FILES) $(HEADERS)
	@mkdir -p $(@D)

	$(CC) $(CPPFLAGS) -c $< -o $@

.PHONY: webservice
webservice: $(OBJECTS) $(HEADERS)
	$(CC) $(CPPFLAGS) $(MAIN_DIR)/WebServiceMain.cpp -o $(BUILD_DIR)/webservice $(OBJECTS) $(wildcard $(SOCKET_OBJECTS)*) $(LDFLAGS)

.PHONY: mnist
mnist: $(OBJECTS) $(HEADERS)
	$(CC) $(CPPFLAGS) $(MAIN_DIR)/Mnist/LocalMnistTraining.cpp -o $(BUILD_DIR)/mnist $(OBJECTS)

.PHONY: tetris
tetris: $(OBJECTS) $(HEADERS)
	$(CC) $(CPPFLAGS) $(MAIN_DIR)/Tetris/LocalTetrisTraining.cpp -o $(BUILD_DIR)/tetris $(OBJECTS)

.PHONY: grid
grid: $(OBJECTS) $(HEADERS)
	$(CC) $(CPPFLAGS) $(MAIN_DIR)/QLearningGrid/QLearningGrid.cpp -o $(BUILD_DIR)/grid $(OBJECTS)

.PHONY: picker
picker: $(OBJECTS) $(HEADERS)
	$(MPI) $(CPPFLAGS) $(MAIN_DIR)/Mnist/MPIEnsembleMnistTraining.cpp -o $(BUILD_DIR)/picker $(OBJECTS)

.PHONY: clean
clean:
	rm -r -f $(BIN_DIR)*