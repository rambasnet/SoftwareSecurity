# rule for compiling program

COMPILER = g++
COMPILER_FLAGS = -c -g -Wall -std=c++17 -m32
BUILD_FLAGS = -m32 -fno-stack-protector -z execstack -no-pie

# list .cpp files separated by space
CPP_FILES = main.cpp

# executable program name
PROGRAM_NAME = tinyweb.exe

.PHONY: all
all: build run

# rule for compiling and building program
# make or make all triggers the following rule
.PHONY: build
build:
	# compiles .cpp to object file .o
	$(COMPILER) $(COMPILER_FLAGS) $(CPP_FILES)
	# builds executable from object files
	$(COMPILER) $(BUILD_FLAGS) -o $(PROGRAM_NAME) *.o
	chown root:root $(PROGRAM_NAME)
	chmod u+s $(PROGRAM_NAME)


# rule for running programming
# make run triggers the following rule
.PHONY: run
run:
	./$(PROGRAM_NAME)

# rule for clean up
# make clean triggers the following rule
.PHONY: clean
clean:
	rm -f $(PROGRAM_NAME) *.o
	
