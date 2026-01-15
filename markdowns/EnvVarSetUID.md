# Set-UID Programs and Environment Variables

## Need for Privileged Programs

- many system programs and configuration files are root/admin owned with restricted access to regular users
- some of these programs need to be executed by regular users regular usage of the system

## Password Dilemma

- let's see the permission of `/etc/shadow` file
- how do regular users change their password?


```python
! ls -al /etc/shadow
```


```python
! getfacl /etc/shadow
```

## Two-Tier Approach

- implementing fine-grained access control in OS make the over complicated
    - requires major redesign/rewrite of all major OS kernels
- OS relies on extension to enforce fine grained access control
- privileged programs are such extensions

![image.png](./media/setuid-approach.png)

- there are two types of privileged programs:

### Daemons
- computer program/process that runs in the background
- typically needs to run as root or other privileged users


### Set-UID programs
- widely used in \*NIX systems
- programs are marked with a speical bit

## Setuid Programs

- setuid programs are a device to allow users to acquire new priviledges for a limited amunt of time
- they provide a meas for overriding the protection schemes designed in \*NIX systems
- `setuid` programs are written in a way that it provides a granual access to individual line of a protected file/resource
- powersuit analogy - suit that gives power to whoever uses it
- which of the following group is the power suit?
    - superman/spiderman suit?
    - ironman/antman suit?
    
- allow users to run a program with the program owner's privilege
- the privilege is temporary while the program is being executed

### Case study - passwd program
- **passwd** program is used to change user's password
- let's see the access control list of **passwd** program
- note `s` in ACL and the owner:group


```python
! ls -l /usr/bin/passwd
```


```python
! getfacl /usr/bin/passwd
```

## Set-UID Concept

- every process has two User IDs:

#### Real UID (RUID)
- identifies the real owner of the process

#### Effective UID (EUID)
- identifies privilege of a process
- access control is based on **EUID**

- when a normal program is executed, **RUID = EUID**
    - both IDs equal to the ID of the user who runs the program
- when a **set-uid** is executed, **RUID != EUID**
    - RUID still equals to user's ID who runs the program
    - EUID howover is equal to the program owner's ID
    - if the program is owned by root, the program runs with the root privilege
    
### meowcat - copy cat
- create a program that behaves like the `cat` program
- the demo program is provided in `demos/setuid` folder
- let's change the working diectory to `demos/setuid`
- use jupyter notebook's magic symbol to do that: `%`


```python
%pwd
```


```python
%cd demos/setuid
```


```python
# if you get a warning when using magic symbol...
# UserWarning: using dhist requires you to install the `pickleshare` library.
! pip install pickleshare
```


```python
! ls -al
```


```python
! cat meowcat.cpp
```


```python
! cat Makefile
```


```python
! make
```


```python
! ./meowcat
```


```python
! ls -al
```


```python
! getfacl meowcat
```


```python
! echo "hello there..." > data.txt
```


```python
! cat data.txt
```


```python
! ./meowcat data.txt
```


```python
! ./meowcat /etc/shadow
```


```python
! ls -al 
```


```python
! getfacl $(which sudo)
```


```python
# let's set the ownership of the program to root to setuid
! echo kali | sudo -S chown root meowcat
```


```python
! ls -al meowcat
```


```python
! getfacl meowcat
```


```python
# let's setuid chmod 4755
! echo kali | sudo -S chmod u+s meowcat
```


```python
! ls -al meowcat
```


```python
! cat data.txt
```


```python
! ./meowcata data.txt
```


```python
# if the owner isn't root or privileged user, the program is setuid but NOT privilege
! echo kali | sudo -S chown user meowcat
```


```python
! ls -al meowcat
```


```python
! echo kali | sudo -S chmod 4777 meowcat
```


```python
! ls -al meowcat
```


```python
! ./meowcat /etc/shadow
```

## What programs need setuid?

- programs that require to access sensitive, system-wide data, files, configs, etc.
- is it safe to setuid every programs?
    - /bin/cat, /bin/sh, nano, vi, etc.


## Environment Variables

- environment variables are integral part of all OS
- these are dynamic values that can have significant influence over the behavior of programs and processes
- these variables are outside the program that store configuration or settings and their values can be changed without modifying the programs' actual code 
- one of the most important `env` variables is `PATH`
    - PATH stores the locations to search for various binaries, libraries, and executable programs
- programs and scripts can access environment variables to adapt their behavior based on the configuration and requirements of the system they're being executed
- OSes provide various ways to set and access environment variables
- system APIs are available to programming languages to access and set environment variables in the programs
- in Linux, env variables play important roles when using Terminals to run programs
- `env` or `printenv` commands list all the environment variables available to the current user
- variable names are case sensitive
- normally ALL_CAPS are used for env variable names


```python
#! /usr/bin/env python

print('hello world')
```


```python
! /usr/bin/env python
```

### Setting and Accessing Environment Variables

- env variables can be set in many places depending on the needs

1. System wide
    - /etc/environment
    - /etc/profile
2. User specific
    - ~/.profile
    - ~/.bashrc
    - ~/.zshrc


```python
! cat /etc/environment
```


```python
! echo $HOME
```


```python
! cat $HOME/.zshrc
```


```python
! echo $PATH
```

### Types of Env Variables

#### shell variable
- like local variables in programs; available to the current shell until it's terminated
- can't have trailing and leading space around the '=' sign unlike in many programming languages
- space is a special character -- word/command separater -- in bash scripting

```bash
VAR_NAME="VALUE..."
```

#### environment or system variables
- like global variables; they can be available system-wide across the Terminals
- temporary env variable can be exported from the Terminal and availble only to that Terminal
- permenant env variables are exported from init script such as `~/.zshrc` everytime a Terminal is launched

```bash
export VAR[="VALUE..."]
set VAR1="some other values" # use in scripts
```

#### NOTE
- can't create shell and environemnt variables from Jupyter Notebooks
- use Terminal instead...
- add `$` before the variable name when reading the value


```python
! VAR=test
```


```python
! echo $VAR
```

```bash
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/setuid]
└─$ VAR="Some data"
                                                                                                     
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/setuid]
└─$ echo $VAR            
Some data

(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/setuid]
└─$ cd             
                                                                                                     
(base) ┌──(kali㉿kali)-[~]
└─$ echo $VAR
Some data

(base) ┌──(kali㉿kali)-[~]
└─$ export VAR
                                                                                                     
(base) ┌──(kali㉿kali)-[~]
└─$ env | grep VAR
VAR=Some data
```

### Update existing env variable

- sometime we may need to temporarily and dynamically add a new path to the PATH variable

```bash
(base) ┌──(kali㉿kali)-[~]
└─$ PATH=$PATH:$HOME/bin
                                                                                                     
(base) ┌──(kali㉿kali)-[~]
└─$ export PATH 

# one liner
(base) ┌──(kali㉿kali)-[~]
└─$ export PATH=$PATH:$HOME/bin

(base) ┌──(kali㉿kali)-[~]
└─$ env | grep ^PATH=
PATH=/home/kali/miniconda3/bin:/home/kali/miniconda3/condabin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/local/games:/usr/games:/home/kali//home/kali/bin
```

### Delete environment variables

- can use `unset` command or assign empty value to temporarily delete them
- can update the corresponding setting files to permanently delete them

```
unset NAME
NAME=""
```

# How to access environment variables from C/C++ programs

- you can pass `*char []` to the main as the third argument
- you can use global varaiable `environ`
- you can use system API such as `getenv(var_name)`, `putenv()`, `setenv(), unsetenv()`, etc.
- let's see how they works
- example codes provided in `demos/envvars` folder


```python
%pwd
```


```python
%cd ../demos/envvars
```


```python
! cat pass_env.cpp
```


```python
! g++ -o pass_env pass_env.cpp
```


```python
! ./pass_env hi there world!
```


```python
! cat global_env.cpp
```


```python
! g++ -o global_env global_env.cpp
```


```python
! ./global_env
```


```python
! cat get_set_env.cpp
```


```python
! g++ -o get_set_env get_set_env.cpp
```


```python
! ./get_set_env PATH
```


```python
! ./get_set_env TEST "HELLO THERE"
```


```python
# rerunning the program doesn't find the newly set environement variable
# the program is rerun everytime...
! ./get_set_env TEST
```

## How a process gets its environment variables

- one of the two ways
- if a new process is created using `fork()`, the parent process copies and passes its environment variables to the child process
- if a process runs a new process in itself (using `execve()`), the processe's environment variable is overwritten by the new env vars provided to the new process
    - current environment variables are essentially lost/overwritten
- `execve()` system API has 3 parameters
    - `filename, argv, envp`
- https://man7.org/linux/man-pages/man2/execve.2.html

```cpp
execve(const char* filename, char *const argv[], char *const envp[]);
```
- let's see how `execve` can pass the env variables to processes


```python
! cat execve_passenv.cpp
```


```python
! g++ -o execve_passenv execve_passenv.cpp
```


```python
# no envvariables are printed for default and option 1
! ./execve_passenv
```


```python
! ./execve_passenv 1
```


```python
! ./execve_passenv 2
```


```python
! ./execve_passenv 3
```

## Memory Locations for program arguments and environment variables

- let's see the stack frame of main function

<img src="./media/env_stack.png" width="700">


## Futher Readings and References
- How to Write Setuid Programs - https://nob.cs.ucdavis.edu/~bishop/secprog/1987-sproglogin.pdf
- Setuid BSDI Man page - https://seedsecuritylabs.org/Labs_20.04/Software/Environment_Variable_and_SetUID/files/setuid.pdf
- Setuid Demystified - https://web.ecs.syr.edu/~wedu/minix/projects/setuid_paper.pdf


```python

```
