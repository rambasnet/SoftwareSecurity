COMPILER = g++

COMPILER_FLAGS = -g -Wall -std=c++17

# the build target executable program name of your choice
PROGRAM_NAME = secure_catall

# list all .cpp files separated by space; files must exist!
CPP_FILES = secure_catall.cpp

build:
	$(COMPILER) $(COMPILER_FLAGS) $(CPP_FILES) -o $(PROGRAM_NAME)
	sudo chown root $(PROGRAM_NAME)
	sudo chmod +s $(PROGRAM_NAME)

clean:
	rm -f $(PROGRAM_NAME) *.o
