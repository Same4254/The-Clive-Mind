SRC_DIR := ./src/
BIN_DIR := ./bin/
HEADER_DIR := ./include/

SOURCE_FILES := cpp
HEADER_FILES := hpp
OBJECT_FILES := o

SOCKET_OBJECTS := ./SocketIOcpp/

CC       := g++
MPI      := mpic++
CPPFLAGS := -std=c++17 -O3 -Wall -I $(HEADER_DIR)
LDFLAGS := -lpthread

SOURCES := $(shell find $(SRC_DIR) -name "*.$(SOURCE_FILES)")
HEADERS := $(shell find $(HEADER_DIR) -name "*.$(HEADER_FILES)")
OBJECTS := $(patsubst $(SRC_DIR)%.$(SOURCE_FILES), $(BIN_DIR)%.$(OBJECT_FILES), $(SOURCES))

all: $(OBJECTS) $(HEADERS)

$(BIN_DIR)%$(OBJECT_FILES): $(SRC_DIR)%$(SOURCE_FILES) $(HEADERS)
	@mkdir -p $(@D)

	$(CC) $(CPPFLAGS) -c $< -o $@

.PHONY: webservice
webservice: $(OBJECTS) $(HEADERS)
	$(CC) $(CPPFLAGS) WebServiceMain.cpp -o webservice $(OBJECTS) $(wildcard $(SOCKET_OBJECTS)*) $(LDFLAGS)

.PHONY: mnist
mnist: $(OBJECTS) $(HEADERS)
	$(CC) $(CPPFLAGS) Experiments/Mnist/LocalMnistTraining.cpp -o mnist $(OBJECTS)

.PHONY: tetris
tetris: $(OBJECTS) $(HEADERS)
	$(CC) $(CPPFLAGS) Experiments/Tetris/LocalTetrisTraining.cpp -o tetris $(OBJECTS)

.PHONY: grid
grid: $(OBJECTS) $(HEADERS)
	$(CC) $(CPPFLAGS) Experiments/QLearningGrid/QLearningGrid.cpp -o grid $(OBJECTS)

.PHONY: picker
picker: $(OBJECTS) $(HEADERS)
	$(MPI) $(CPPFLAGS) Experiments/Mnist/MPIEnsembleMnistTraining.cpp -o picker $(OBJECTS)

.PHONY: test
test: $(OBJECTS) $(HEADERS)
	$(CC) $(CPPFLAGS) Tests/FileTest.cpp -o file-test $(OBJECTS)

.PHONY: clean
clean:
	rm -r -f $(BIN_DIR)*