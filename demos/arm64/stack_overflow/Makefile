# It compiles the hello.cpp file into an executable named hello
# It uses the gcc compiler with the -o option to specify the output file
# It also includes the -Wall option to enable all compiler's warning messages
# The -g option is used to include debugging information in the executable
# The -O2 option is used to optimize the code for speed
# The -std=c++11 option is used to specify the C++ standard to use

FILE = overflow.c
EXE = overflow.exe

.PHONY: all
all: clean build

.PHONY: build
build:
	arm-linux-gnueabihf-gcc -marm -o ${EXE} ${FILE} -Wall -g -O2 -fno-stack-protector -no-pie -z execstack

.PHONY: clean
clean:
	rm -f ${EXE}

