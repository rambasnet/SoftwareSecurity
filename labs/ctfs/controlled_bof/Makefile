# rule for compiling program

COMPILER = gcc
COMPILER_FLAGS = -c -g -Wall -m32
BUILD_FLAGS = -m32 -fno-stack-protector -z execstack -no-pie

# list .cpp files separated by space
CPP_FILES = $(wildcard *.c)

# executable program name
PROGRAM_NAME = bof.exe

# rule for compiling and building program
# make or make all triggers the following rule
.PHONY: build
build:
	# compiles .cpp to object file .o
	$(COMPILER) $(COMPILER_FLAGS) $(CPP_FILES)
	# builds executable from object files
	$(COMPILER) $(BUILD_FLAGS) -o $(PROGRAM_NAME) *.o
	# disable ASLR
	echo 0 | sudo tee /proc/sys/kernel/randomize_va_space

# rule for running programming
# make run triggers the following rule
.PHONY: run
run: build
	./$(PROGRAM_NAME)

# rule for clean up
# make clean triggers the following rule
.PHONY: clean
clean:
	rm -f $(PROGRAM_NAME) *.o
	
