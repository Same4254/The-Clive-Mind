SRC_DIR := ./src/
BIN_DIR := ./bin/
HEADER_DIR := ./include/

SOURCE_FILES := cpp
HEADER_FILES := hpp
OBJECT_FILES := o

SOCKET_OBJECTS := ./SocketIOcpp/

CC       := g++
MPI      := mpic++
CPPFLAGS := -std=c++17 -g -Wall -I $(HEADER_DIR) -ftrapv -D_GLIBCXX_DEBUG
LDFLAGS  := -lpthread

SOURCES := $(shell find $(SRC_DIR) -name "*.$(SOURCE_FILES)")
HEADERS := $(shell find $(HEADER_DIR) -name "*.$(HEADER_FILES)")
OBJECTS := $(patsubst $(SRC_DIR)%.$(SOURCE_FILES), $(BIN_DIR)%.$(OBJECT_FILES), $(SOURCES))

all: $(OBJECTS) $(HEADERS)

$(BIN_DIR)%$(OBJECT_FILES): $(SRC_DIR)%$(SOURCE_FILES) $(HEADERS)
	@mkdir -p $(@D)

	$(CC) $(CPPFLAGS) -c $< -o $@

.PHONY: game
game: $(OBJECTS) $(HEADERS)
	$(CC) $(CPPFLAGS) main.cpp -o game $(OBJECTS) $(wildcard $(SOCKET_OBJECTS)*) $(LDFLAGS)

.PHONY: clean
clean:
	rm -r -f $(BIN_DIR)*
