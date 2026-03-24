COMPILER = g++

COMPILER_FLAGS = -g -Wall -m32 -Wall -Wpedantic -Wextra -Wconversion -fno-stack-protector -z execstack -no-pie

# the build target executable program name of your choice
PROGRAM_NAME = argv.exe

# list all .cpp files separated by space; files must exist!
CPP_FILES = argv.cpp

.PHONY: build
build_target:
	$(COMPILER) $(COMPILER_FLAGS) $(CPP_FILES) -o $(PROGRAM_NAME)
	#sudo chown root:root $(PROGRAM_NAME)
	#sudo chmod +s $(PROGRAM_NAME)

build_exploit:
	$(COMPILER) $(COMPILER_FLAGS) argv_exploit.cpp -o argv_exploit.exe

.PHONY: clean
clean:
	rm -f $(PROGRAM_NAME) *.o
