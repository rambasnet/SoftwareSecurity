COMPILER = gcc
COMPILER_FLAGS = -g -Wall -m32 -fno-stack-protector -z execstack -no-pie

# list .cpp files separated by space
CPP_FILES = $(wildcard *.c)

# executable program name
PROGRAM_NAME = basic_bof.exe

# make or make bild triggers the following rule
.PHONY: build
build:
	$(COMPILER) $(COMPILER_FLAGS) $(CPP_FILES) -o $(PROGRAM_NAME)
	# disable ASLR
	@echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
	@echo "Built and disabled randomize_va_space!"

# make clean triggers the following rule
.PHONY: clean
clean:
	rm -f $(PROGRAM_NAME) *.o *.exe
	
