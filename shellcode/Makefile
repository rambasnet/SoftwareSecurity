
all: 32bit 64bit

clean:
	rm -f *.out

32bit:
	g++ -m32 -z execstack -o callshell32.out call_shellcode.cpp
	@echo callshell32.out built successfully!

64bit:
	g++ -z execstack -o callshell64.out call_shellcode.cpp
	@echo callshell64.out built successfully!

setuid: all
	sudo chown root:root callshell32.out callshell64.out
	sudo chmod u+s callshell32.out callshell64.out
	@echo setuid set for both 32-bit and 64-bit exectuables!
