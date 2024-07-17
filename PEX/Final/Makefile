# File names
FNAME = sperber2-part1
FNAME2 = sperber2-part2
FNAME3 = sperber2-part3

# Arguments
ARGS1 = --help
ARGS2 = --get-metacyc RXN12345 go-basic.obo
ARGS3 = --tab-metacyc go-basic.obo

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall

# Default target
.PHONY: default
default: all

# Targets for each part
.PHONY: part1
part1: $(FNAME).cpp
	$(CXX) $(CXXFLAGS) $(FNAME).cpp -o ex1

.PHONY: part2
part2: $(FNAME2).cpp
	$(CXX) $(CXXFLAGS) $(FNAME2).cpp -o ex2 && sleep 0.2s && ./ex2 $(ARGS2)

.PHONY: part3
part3: $(FNAME3).cpp
	$(CXX) $(CXXFLAGS) $(FNAME3).cpp -o ex3 && sleep 0.2s && ./ex3 $(ARGS3)

# Target to build all parts
.PHONY: all
all: part1 part2 part3

# Clean up build files
.PHONY: clean
clean:
	rm -f ex1 ex2 ex3