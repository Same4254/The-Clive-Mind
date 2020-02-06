CC = g++
CPPFLAGS = -Wall -std=c++17 -O3 -I include/

SOURCES := $(shell find src/ -type f -iname '*.cpp')
OBJECTS := $(foreach x, $(basename $(SOURCES)), $(x).o)

all: $(OBJECTS)

%.o: %.cpp
	$(CC) -c $(CPPFLAGS) $^ -o $(patsubst %.cpp, build/%.o, $(notdir $^))

.PHONY: mnist
mnist: $(OBJECTS)
	$(CC) $(CPPFLAGS) Experiments/Mnist/mnistTraining.cpp -o mnist-test $(patsubst %.cpp, build/%.o, $(notdir $(SOURCES)))

.PHONY: clean
clean:
	rm build/*