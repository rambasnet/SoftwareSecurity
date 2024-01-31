COMPILER = g++

COMPILER_FLAGS = -g -Wall -std=c++17

# the build target executable program name of your choice
PROGRAM_NAME = catall

# list all .cpp files separated by space; files must exist!
CPP_FILES = catall.cpp

.PHONY: build
build:
	$(COMPILER) $(COMPILER_FLAGS) $(CPP_FILES) -o $(PROGRAM_NAME)
	sudo chown root $(PROGRAM_NAME)
	sudo chmod +s $(PROGRAM_NAME)

.PHONY: clean
clean:
	rm -f $(PROGRAM_NAME) *.o
# make clean rule