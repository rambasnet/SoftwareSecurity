# rule for compiling program

COMPILER = g++
BUILD_FLAGS = -m32 -g -Wall -std=c++17 -fno-stack-protector -z execstack -no-pie

# list .cpp files separated by space
CPP_FILES = $(wildcard *.cpp)

# executable program name
PROGRAM_NAME = log_server.exe

# rule for compiling and building program
# make or make all triggers the following rule
build:
	# disable ASLR
	#echo 0 | tee /proc/sys/kernel/randomize_va_space
	# builds executable from object files
	$(COMPILER) $(BUILD_FLAGS) -o $(PROGRAM_NAME) $(CPP_FILES)
	sudo chown root:root $(PROGRAM_NAME)
	sudo chmod u+s $(PROGRAM_NAME)


# rule for running programming
# make run triggers the following rule
run:
	./$(PROGRAM_NAME)

# rule for clean up
# make clean triggers the following rule
clean:
	rm -f $(PROGRAM_NAME) *.o
	
