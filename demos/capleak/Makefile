COMPILER = g++

COMPILER_FLAGS = -g -Wall -std=c++17

# the build target executable program name of your choice
PROGRAM_NAME = capleak

CPP_FILES = capleak.cpp

all: build
	sudo chown root $(PROGRAM_NAME)
	sudo chmod +s $(PROGRAM_NAME)


build:
	$(COMPILER) $(COMPILER_FLAGS) $(CPP_FILES) -o $(PROGRAM_NAME)

# make clean rule
# delete object and binary files
clean:
	rm -f $(PROGRAM_NAME) *.o
