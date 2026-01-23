# Makefile to compile hello.cpp

# This Makefile is for the arm64 architecture
# It compiles the hello.cpp file into an executable named hello
# It uses the g++ compiler with the -o option to specify the output file
# It also includes the -Wall option to enable all compiler's warning messages
# The -g option is used to include debugging information in the executable
# The -O2 option is used to optimize the code for speed
# The -std=c++11 option is used to specify the C++ standard to use

.PHONY: all
all: clean build run
	@echo "Build complete. Run ./hello to execute."

.PHONY: build
build:
	g++ -o hello.exe hello.cpp -Wall -g -O2 -std=c++11

.PHONY: clean
clean:
	rm -f hello.exe
	rm -f hello.o
	rm -f hello

.PHONY: run
run: build
	@echo "Running hello.exe..."
	./hello.exe
	@echo "Execution complete."