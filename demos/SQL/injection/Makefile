# Makefile Template
# variable for compiler name
COMPILER = g++

# c++ compiler flags
# -c : just do preprocess; compile and assemble into object files
# -g : adds debugging information to the executable file
# -Wall : turns on most, but not  all, compiler warnings
# -std=c++17 : uses c++17 standard
# put all the required flags separated by space
COMPILER_FLAGS = -g -Wall -std=c++17 -fno-stack-protector -z execstack -no-pie

# the build target executable program name of your choice
PROGRAM_NAME = lucky7.exe # FIXME

# list all .cpp files separated by space; files must exist!
CPP_FILES = lucky7.cpp main.cpp security.cpp db.cpp # FIXME

# make build rule/default rule that'll be run when we run "make" or make build
# first compile each file to object file
# then make a single binary program
build:
	# must run make with sudo to disable randomaize_va_space
	echo 0 | tee /proc/sys/kernel/randomize_va_space
	$(COMPILER) $(COMPILER_FLAGS) $(CPP_FILES) -o $(PROGRAM_NAME) -lcrypto -lssl -lsqlite3

# make clean rule
# delete object and binary files
clean:
	rm -f $(PROGRAM_NAME) *.o
