# rule for compiling program

COMPILER = gcc
COMPILER_FLAGS = -g -Wall -m32 -fno-stack-protector -z execstack -no-pie

# list .cpp files separated by space
CPP_FILES = $(wildcard *.c)

# executable program name
PROGRAM_NAME = simple_rip.exe

# rule for compiling and building program
# make or make all triggers the following rule
.PHONY: build
build:
	$(COMPILER) $(COMPILER_FLAGS) $(CPP_FILES) -o $(PROGRAM_NAME)
	# disable ASLR when the environment allows it
	@if [ -w /proc/sys/kernel/randomize_va_space ]; then \
		echo 0 > /proc/sys/kernel/randomize_va_space; \
	elif command -v sudo >/dev/null 2>&1 && sudo -n test -w /proc/sys/kernel/randomize_va_space 2>/dev/null; then \
		echo 0 | sudo -n tee /proc/sys/kernel/randomize_va_space >/dev/null; \
	else \
		echo "Skipping ASLR disable: /proc/sys/kernel/randomize_va_space is not writable in this environment."; \
	fi

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
	
