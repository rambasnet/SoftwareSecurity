COMPILER = g++

COMPILER_FLAGS = -g -Wall -std=c++17

# the build target executable program name of your choice
PROGRAM_NAME = capleak

CPP_FILES = capleak.cpp

.PHONY: all
all: build
	sudo chown root $(PROGRAM_NAME)
	sudo chmod +s $(PROGRAM_NAME)

.PHONY: build
build:
	$(COMPILER) $(COMPILER_FLAGS) $(CPP_FILES) -o $(PROGRAM_NAME)


# delete object and binary files
.PHONY: clean
clean:
	rm -f $(PROGRAM_NAME) *.o
