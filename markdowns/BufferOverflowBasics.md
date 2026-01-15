# Buffer Overflow

- one of the top 25 most dangerous software errors according to Common Weakness Enemuration (CWE) for several years: https://cwe.mitre.org/data/index.html - See CWE Top 25 - Out-of-bounds Write (CWE-787)
- most Internet worms use buffer overflow vulnerabilities to propagate
- C and C++ assume that programmers are responsible for data integrity
- modern compilers and OSes provide various protections making it harder to exploit traditional buffer overflows
- once the buffer/memory is allocated, there's no built-in mechanism to safeguard the amount of data that can be stored into
    - this is called buffer overrun or buffer overflow
    - user data may overwrite other part of the memory it's not supposed to making the program behave differently or crashing it
    - attackers could take advantage of this flaw to control the program and may pawn the system by executing shellcode or payload of their choice
- buffer overflow can violate all three core principles of **CIA** triad - **Confidentiality, Integrity, and Availability**
- **Confidentiality**: allows attackers to read confidential or data that they're not supposed to have access to
- **Integrity**: allows attackers to modify part of the data/program that they're not supposed to
- **Availability**: allows attackes to crash the system (SEGFAULT) violating availbility of the service

- buffer can be overflown in all memory segments (Global, Stack and Heap)
- Stack overflow can have the most significant consequences!

## Disable ASLR and other compiler protections
- compile.sh script disables ASLR and compiler protections


```python
# if value is 0; ASLR is disabled
! cat /proc/sys/kernel/randomize_va_space
```

    2



```python
# disable ASLR if randomize_va_space is not 0
! echo kali | sudo -S bash -c 'echo 0 | sudo -S tee /proc/sys/kernel/randomize_va_space'
```

    [sudo] password for kali: 0


## Program's memory and stack may shift
- programs and data must be loaded into RAM inorder to execute
- depending on how the program is launched, several factors make the program's location in memory shift
    - even after virtual-address space layout randomization is disabled
- e.g. environment variables, length of program name, arguments to program, etc.
- gdb also sets its own environment variables such as LINES, COLUMNS, etc.
<img src="media/gdb-env.png">
- detail: https://stackoverflow.com/questions/17775186/buffer-overflow-works-in-gdb-but-not-without-it/17775966#17775966
- let's see `demos/env_demo.cpp` for demonstration


```python
! cat demos/env_demo.cpp
```

    #include <cstdio>
    #include <stdlib.h> // getenv, putenv, setenv, unsetenv, etc.
    // see: http://www.manpagez.com/man/3/getenv/
    
    int main(int argc, char* argv[]) {
      if (argc == 2) {
        // read value of env variable
        //getenv(char * name) retruns NUL-terminated string; if name doesn't exists, returns NULL
        printf("%s=%s\n", argv[1], getenv(argv[1]));
      }
      else if (argc == 3) { 
        // set env variable, 0 doesn't overwrite, 1 overwrites
        int result = setenv(argv[1], argv[2], 0); // returns 0 on successful, -1 otherwise
        if (result == 0) {
          printf("Successfully set env variable!\n");
          printf("%s=%s\n", argv[1], getenv(argv[1]));
          // you can't use setenv to export variables from current process to the calling process (shell)
        }
        else {
          printf("Couldn't set env variable!");
        }
      }
      else {
        printf("%s\n", "Usage READ: program ENV_VAR_NAME");
        printf("%s\n", "Usage SET: program ENV_VAR_NAME VALUE");
      }
      return 0;
    }



```python
# let's compile the program as 32-bit
! g++ -m32 demos/env_demo.cpp -o env_demo.exe
```


```python
# check the exe program
! ls -al env_demo.exe
```

    -rwxr-xr-x 1 kali kali 15228 Feb 10 09:24 env_demo.exe



```python
# run the program
! ./env_demo.exe
```

    Usage READ: program ENV_VAR_NAME
    Usage SET: program ENV_VAR_NAME VALUE



```python
! ./env_demo.exe PATH
```

    PATH=/home/kali/miniconda3/bin:/home/kali/miniconda3/condabin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/local/games:/usr/games



```python
! ./env_demo.exe USER
```

    USER=kali



```python
! ./env_demo.exe SOME_VAR
```

    SOME_VAR=(null)



```python
# set new env varirable for current projcess and it's children using fork
! ./env_demo.exe SOME_VAR "THIS IS THE VALUE FOR SOME_VAR"
```

    Successfully set env variable!
    SOME_VAR=THIS IS THE VALUE FOR SOME_VAR


## Observe difference between so_arg.cpp and so_stdio.cpp
- `demos/stack_overflow/so_stdio.cpp` doesn't take/use any argument
    - interactively prompts user to enter some text duing program execution
- `demos/stack_overflow/so_arg.cpp` takes argument
    - uses argument provided to the program
    - not an interactive program
- let's compile and run these programs to observe the address of buffer


```python
! cat demos/stack_overflow/so_arg.cpp
```

    #include <cstdio>
    #include <cstring>
    #include <iostream>
    
    using namespace std;
    
    #define BUFSIZE 128
    
    void copydata(char *dst_buf, char *src_buf) {
        // similar to strcpy in cstdio
        size_t i = 0;
        // copy each character from src_buf to des_buffer
        // stop when NUL character is encountered
        while (*(src_buf+i) != 0) {
            *(dst_buf+i) = *(src_buf+i);
            i++;
        }
        *(dst_buf+i) = 0; // end with NUL character
    }
    
    void bad(char *data) {
        char buffer[BUFSIZE];
        printf("buffer is at %p\n", buffer);
        //strcpy(buffer, data);
        copydata(buffer, data);
        cout << "buffer contains: \n" << buffer << endl;
    }
    
    int main(int argc, char *argv[]) {
        bad(argv[1]);
        cout << "Good bye!" << endl;
        return 0;
    }

- let's compile the program and run it
- use the provided compile.sh bash program
    - helps in compiling programs disabling all the overflow-related security controls


```python
# let's see the script
! cat ./compile.sh
```

    #! /bin/bash
    
    # disable aslr
    read aslr < /proc/sys/kernel/randomize_va_space
    if (( $aslr != 0 )); then
        if (( id -u != 0)); then
            echo "Need sudo priviledge to disable aslr..."
        fi
        # provide password for sudo user
        ls /proc/sys/kernel/randomize_va_space
        echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
    fi
    
    # compile disabling all the security flags
    if (( $# == 1 )); then
        g++ -g -m32 -std=c++17 -fno-stack-protector -z execstack -no-pie $1 && echo "Output file is a.out"
    elif (( $# == 2 )); then
        g++ -g -m32 -std=c++17 -fno-stack-protector -z execstack -no-pie -o $2 $1
    else
        echo "Error... No input file provided..."
        echo "$0 <inputfile.cpp> [a.out]"
    fi


```python
! echo kali | sudo -S ./compile.sh demos/stack_overflow/so_arg.cpp so_arg.exe
```

    [sudo] password for kali: 


```python
# provide the argument to program
! ./so_arg.exe "Hello World!"
# Note the address of buffer!
```

    buffer is at 0xffffc240
    buffer contains: 
    Hello World!
    Good bye!



```python
# provide different argument with the same length to program
! ./so_arg.exe "HELLO WORLD!"
# Note the address of buffer!
```

    buffer is at 0xffffc240
    buffer contains: 
    HELLO WORLD!
    Good bye!



```python
! ./so_arg.exe $(python -c 'print("A"*100)')
# Note the address of buffer!
```

    buffer is at 0xffffc1e0
    buffer contains: 
    AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    Good bye!



```python
! ./so_arg.exe $(python -c 'print("A"*200)')
# Note the address of buffer!
```

    buffer is at 0xffffc180
    buffer contains: 
    AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA


- Notice the different address for buffer in so_arg.exe based on the length of argument
- the address of the buffer shifts within the program
- this makes it a little difficult to exploit the program if the argument length varies

## buffer address in so_stdio.cpp
- compile and run the program and notice the address of buffer


```python
! cat demos/stack_overflow/so_stdio.cpp
```

    #include <stdio.h>
    #include <string.h>
    #include <sys/types.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <iostream>
    using namespace std;
    
    #define BUFSIZE 128
    
    using namespace std;
    
    void give_shell(){
        system("/bin/sh");
    }
    
    char * mgets(char *dst) {
        char *ptr = dst;
        int ch; 
    	/* skip leading white spaces */ 
        while ((ch = getchar()) && (ch == ' ' or ch == '\t') )
            ; 
    
        if ((ch == '\n') or (ch == EOF)) { 
            *ptr = '\0';
            return dst; 
        } 
        else
            *ptr = ch;
    
        /* now read the rest until \n or EOF */ 
        while (true) {
            ch = getchar();
            if (ch == '\n' or ch == EOF) break;
            *(++ptr) = ch; 
        }
        *(++ptr) = 0;
        return dst;
    }
    
    void bad() {
        char buffer[BUFSIZE];
        printf("buffer is at %p\n", buffer);
        cout << "Give me some text: ";
        fflush(stdout);
        mgets(buffer); // similar to C's gets;
        //gets(buffer); // depricated
        cout << "Acknowledged: " << buffer << " with length " << strlen(buffer) << endl;
    }
    
    int main(int argc, char *argv[]) {
        bad();
        cout << "Good bye!\n";
        return 0;
    }



```python
! echo kali | sudo -S ./compile.sh demos/stack_overflow/so_stdio.cpp so_stdio.exe
```

    [sudo] password for kali: 


```python
# Must pipe the input or interactively type
! echo "Hello World" | ./so_stdio.exe 
# Note the address of buffer
```

    buffer is at 0xffffc240
    Give me some text: Acknowledged: Hello World with length 11
    Good bye!



```python
# run the progam with the same length argument of input
! echo "World_Hello" | ./so_stdio.exe
# Note the address of buffer; no change because same program is run with the same length of argument
```

    buffer is at 0xffffc240
    Give me some text: Acknowledged: World_Hello with length 11
    Good bye!



```python
# run the program with 100 As
! python -c 'print("A"*100)' | ./so_stdio.exe
# Note the address of buffer!
```

    buffer is at 0xffffc240
    Give me some text: Acknowledged: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA with length 100
    Good bye!



```python
# run the program with 200 As
! python -c 'print("A"*200)' | ./so_stdio.exe
# Note the address of buffer!
```

    buffer is at 0xffffc240
    Give me some text: Acknowledged: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA with length 200


#### Note:
- as demonstrated above, length of data read from standard input doesn't shift the addresses of the buffer/variables

## length of the program name
- let's give names of different lengths to the same program and notice the address of buffer being shifted with the program
- this is true regardless the program takes an argument or not 
    - NOTE the name of the program itself is passed as an argument to the program
    - the first argument to main's argv[] is the name of the program that gets pushed on to the main's stack


```python
# let's compile the program and run the program with name stack_overflow_stdio.exe
! echo kali | sudo -S ./compile.sh demos/stack_overflow/so_stdio.cpp stack_overflow_stdio.exe
```

    [sudo] password for kali: 


```python
! echo "Hello_World" | ./stack_overflow_stdio.exe
```

    buffer is at 0xffffc2a0
    Give me some text: Acknowledged: Hello_World with length 11
    Good bye!



```python
# if the same program is run multiple times, 
! echo "Hello_Worl adfadsfasfasfasf asfadsfasdfadsfdfasfsa d" | ./stack_overflow_stdio.exe
# Notice no change in buffer address
```

    buffer is at 0xffffc2a0
    Give me some text: Acknowledged: Hello_Worl adfadsfasfasfasf asfadsfasdfadsfdfasfsa d with length 52
    Good bye!



```python
# let's compile nd run the program with name stack.exe
! echo kali | sudo -S ./compile.sh demos/stack_overflow/so_stdio.cpp stack.exe
```

    [sudo] password for kali: 


```python
! echo "Hello_World" | ./stack.exe
```

    buffer is at 0xffffc2d0
    Give me some text: Acknowledged: Hello_World with length 11
    Good bye!



```python
# let's compile and run the program with the name stack1.exe
! echo kali | sudo -S ./compile.sh demos/stack_overflow/so_stdio.cpp stack1.exe
# minor change in length may not shift it
```

    [sudo] password for kali: 


```python
! echo "Hello_World" | ./stack1.exe
```

    buffer is at 0xffffc2d0
    Give me some text: Acknowledged: Hello_World with length 11
    Good bye!



```python
# let's compile the program and run the program with the name stack11.exe
! echo kali | sudo -S ./compile.sh demos/stack_overflow/so_stdio.cpp stack11.exe
```

    [sudo] password for kali: 


```python
! echo "Hello_World" | ./stack11.exe
# perhaps, filename is not long engouh to make a difference
```

    buffer is at 0xffffc2d0
    Give me some text: Acknowledged: Hello_World with length 11
    Good bye!



```bash
%%bash
# let's compile the program with output program name stack11111.exe
echo kali | sudo -S ./compile.sh demos/stack_overflow/so_stdio.cpp stack11111.exe
echo "Hello_World" | ./stack11111.exe
```

    buffer is at 0xffffc2e0
    Give me some text: Acknowledged: Hello_World with length 11
    Good bye!


    [sudo] password for kali: 


```python
# let's find the no. of bytes the buffer address was shifted by
print(0xffffc2e0-0xffffc2d0)
```

    16


## Run program with modified environment
- **env** command let's you run the program in a modified environment
- **env -i** - ignores environment or starts a program without one


```python
! echo Hello | ./stack_overflow_stdio.exe
```

    buffer is at 0xffffc2a0
    Give me some text: Acknowledged: Hello with length 5
    Good bye!



```python
! echo Hello | env -i ./stack_overflow_stdio.exe
```

    buffer is at 0xffffdd90
    Give me some text: Acknowledged: Hello with length 5
    Good bye!



```python
# let's look at the current environment
! env
```

    COLORFGBG=15;0
    DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/1000/bus
    DESKTOP_SESSION=lightdm-xsession
    DISPLAY=:0.0
    GDMSESSION=lightdm-xsession
    HOME=/home/kali
    LANG=en_US.utf8
    LANGUAGE=
    LOGNAME=kali
    PANEL_GDK_CORE_DEVICE_EVENTS=0
    PATH=/home/kali/miniconda3/bin:/home/kali/miniconda3/condabin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/local/games:/usr/games
    PWD=/home/kali/Sp21/EthicalHacking
    QT_ACCESSIBILITY=1
    QT_AUTO_SCREEN_SCALE_FACTOR=0
    QT_QPA_PLATFORMTHEME=qt5ct
    SESSION_MANAGER=local/K:@/tmp/.ICE-unix/852,unix/K:/tmp/.ICE-unix/852
    SHELL=/usr/bin/zsh
    SSH_AGENT_PID=895
    SSH_AUTH_SOCK=/tmp/ssh-1ZCMbOabR0WB/agent.852
    TERM=xterm-color
    USER=kali
    WINDOWID=0
    XAUTHORITY=/home/kali/.Xauthority
    XDG_CONFIG_DIRS=/etc/xdg
    XDG_CURRENT_DESKTOP=XFCE
    XDG_DATA_DIRS=/usr/share/xfce4:/usr/local/share/:/usr/share/:/usr/share
    XDG_GREETER_DATA_DIR=/var/lib/lightdm/data/kali
    XDG_MENU_PREFIX=xfce-
    XDG_RUNTIME_DIR=/run/user/1000
    XDG_SEAT=seat0
    XDG_SEAT_PATH=/org/freedesktop/DisplayManager/Seat0
    XDG_SESSION_CLASS=user
    XDG_SESSION_DESKTOP=lightdm-xsession
    XDG_SESSION_ID=2
    XDG_SESSION_PATH=/org/freedesktop/DisplayManager/Session0
    XDG_SESSION_TYPE=x11
    XDG_VTNR=7
    _JAVA_OPTIONS=-Dawt.useSystemAAFontSettings=on -Dswing.aatext=true
    SHLVL=1
    OLDPWD=/home/kali/Sp21/EthicalHacking
    LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.webp=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.m4a=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.oga=00;36:*.opus=00;36:*.spx=00;36:*.xspf=00;36:
    LESS_TERMCAP_mb=[1;31m
    LESS_TERMCAP_md=[1;36m
    LESS_TERMCAP_me=[0m
    LESS_TERMCAP_so=[01;33m
    LESS_TERMCAP_se=[0m
    LESS_TERMCAP_us=[1;32m
    LESS_TERMCAP_ue=[0m
    CONDA_EXE=/home/kali/miniconda3/bin/conda
    _CE_M=
    _CE_CONDA=
    CONDA_PYTHON_EXE=/home/kali/miniconda3/bin/python
    CONDA_SHLVL=1
    CONDA_PREFIX=/home/kali/miniconda3
    CONDA_DEFAULT_ENV=base
    CONDA_PROMPT_MODIFIER=
    CONDA_BACKUP_HOST=x86_64-conda_cos6-linux-gnu
    HOST=K
    ADDR2LINE=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-addr2line
    AR=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-ar
    AS=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-as
    CXXFILT=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-c++filt
    ELFEDIT=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-elfedit
    GPROF=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-gprof
    LD=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-ld
    LD_GOLD=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-ld.gold
    NM=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-nm
    OBJCOPY=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-objcopy
    OBJDUMP=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-objdump
    RANLIB=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-ranlib
    READELF=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-readelf
    SIZE=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-size
    STRINGS=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-strings
    STRIP=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-strip
    CC=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-cc
    CPP=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-cpp
    GCC=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-gcc
    GCC_AR=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-gcc-ar
    GCC_NM=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-gcc-nm
    GCC_RANLIB=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-gcc-ranlib
    CPPFLAGS=-DNDEBUG -D_FORTIFY_SOURCE=2 -O2 -isystem /home/kali/miniconda3/include
    CFLAGS=-march=nocona -mtune=haswell -ftree-vectorize -fPIC -fstack-protector-strong -fno-plt -O2 -ffunction-sections -pipe -isystem /home/kali/miniconda3/include
    LDFLAGS=-Wl,-O2 -Wl,--sort-common -Wl,--as-needed -Wl,-z,relro -Wl,-z,now -Wl,--disable-new-dtags -Wl,--gc-sections -Wl,-rpath,/home/kali/miniconda3/lib -Wl,-rpath-link,/home/kali/miniconda3/lib -L/home/kali/miniconda3/lib
    DEBUG_CPPFLAGS=-D_DEBUG -D_FORTIFY_SOURCE=2 -Og -isystem /home/kali/miniconda3/include
    DEBUG_CFLAGS=-march=nocona -mtune=haswell -ftree-vectorize -fPIC -fstack-protector-all -fno-plt -Og -g -Wall -Wextra -fvar-tracking-assignments -ffunction-sections -pipe -isystem /home/kali/miniconda3/include
    CMAKE_PREFIX_PATH=/home/kali/miniconda3:/home/kali/miniconda3/x86_64-conda_cos6-linux-gnu/sysroot/usr
    _CONDA_PYTHON_SYSCONFIGDATA_NAME=_sysconfigdata_x86_64_conda_cos6_linux_gnu
    CONDA_BUILD_SYSROOT=/home/kali/miniconda3/x86_64-conda_cos6-linux-gnu/sysroot
    CXX=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-c++
    GXX=/home/kali/miniconda3/bin/x86_64-conda_cos6-linux-gnu-g++
    CXXFLAGS=-fvisibility-inlines-hidden -std=c++17 -fmessage-length=0 -march=nocona -mtune=haswell -ftree-vectorize -fPIC -fstack-protector-strong -fno-plt -O2 -ffunction-sections -pipe -isystem /home/kali/miniconda3/include
    DEBUG_CXXFLAGS=-fvisibility-inlines-hidden -std=c++17 -fmessage-length=0 -march=nocona -mtune=haswell -ftree-vectorize -fPIC -fstack-protector-all -fno-plt -Og -g -Wall -Wextra -fvar-tracking-assignments -ffunction-sections -pipe -isystem /home/kali/miniconda3/include
    _=/usr/bin/env
    JPY_PARENT_PID=9269
    CLICOLOR=1
    PAGER=cat
    GIT_PAGER=cat
    MPLBACKEND=module://ipykernel.pylab.backend_inline



```python
# print the difference in addresses
print(0xffffdd90-0xffffc290)
```

    6912



```python
! ./so_arg.exe Hello
```

    buffer is at 0xffffc2d0
    buffer contains: 
    Hello
    Good bye!



```python
! env -i ./so_arg.exe Hello
```

    buffer is at 0xffffdd90
    buffer contains: 
    Hello
    Good bye!



```python
# print the difference in addresses
print(0xffffdd90-0xffffc2c0)
```

    6864



```python
# argument length and program length still matters
# let's try with longer argument to demonstrate the shift in buffer's address
! env -i ./so_arg.exe "Hello there beautiful world!"
```

    buffer is at 0xffffdd80
    buffer contains: 
    Hello there beautiful world!
    Good bye!



```python

```
