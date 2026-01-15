# Setup Hacking VM

- install virtualization tool
    - VirtualBox (http://www.virtualbox.org)
    - VMWare (https://www.vmware.com)
    - either one is fine; depending on your system VMWare may not be free!
    
- download pre-built virtual images of Kali Linux: https://www.offensive-security.com/kali-linux-vm-vmware-virtualbox-image-download/
- can download either 64-bit or 32-bit (64-bit is recommended)
- if you can provide more than 4 GB of memory (>=50% of total memory your system has) to your virtual machine, you should download 64-bit


```bash
%%bash
# check machine architecture
uname -a
```

    Linux kali 5.7.0-kali1-amd64 #1 SMP Debian 5.7.6-1kali2 (2020-07-01) x86_64 GNU/Linux



```python
! echo kali | sudo -S apt update
```

    Hit:1 http://dl.google.com/linux/chrome/deb stable InRelease                   [0m
    Get:2 https://packages.microsoft.com/repos/vscode stable InRelease [3,958 B]   [0m[33m
    Get:4 https://packages.microsoft.com/repos/vscode stable/main amd64 Packages [215 kB][33m
    Get:3 http://kali.download/kali kali-rolling InRelease [30.5 kB]
    Get:5 http://kali.download/kali kali-rolling/main amd64 Packages [17.0 MB]
    Get:6 http://kali.download/kali kali-rolling/contrib amd64 Packages [104 kB]
    Get:7 http://kali.download/kali kali-rolling/non-free amd64 Packages [200 kB]
    Fetched 17.5 MB in 4s (4,090 kB/s)m                       [0m[33m[33m[33m
    Reading package lists... Done
    Building dependency tree       
    Reading state information... Done
    921 packages can be upgraded. Run 'apt list --upgradable' to see them.


## Compilers and Libraries required
- GCC for compiling C code
- most system programs are written in C language
- G++ for compiling C++ code
- C++ is built on C and C libraries can be readily used in C++
- we'll rely heavily on C++ with essential C libraries and functions for writing vulnerable programs
- both compilers and necessary build tools can be installed by installing **build-essential** package
- https://packages.ubuntu.com/xenial/build-essential
- multilib library to compile x86 programs in x64-bit OS
- we'll use C++ code with the mix of some C code especially to demonstrate various vulnerabilities


```bash
%%bash
echo kali | sudo -S apt install -y curl build-essential ccache gdb
```

    Reading package lists...
    Building dependency tree...
    Reading state information...
    build-essential is already the newest version (12.8).
    The following packages were automatically installed and are no longer required:
      libcdio18 libmpdec2 libprotobuf22 libx264-155 libx264-159
    Use 'sudo apt autoremove' to remove them.
    The following additional packages will be installed:
      libcurl4 libdebuginfod1 libdw1 libelf1 libnsl2 libpython3.9
      libpython3.9-minimal libpython3.9-stdlib
    Suggested packages:
      distcc | icecc gdb-doc gdbserver
    The following NEW packages will be installed:
      libdebuginfod1 libnsl2 libpython3.9 libpython3.9-minimal libpython3.9-stdlib
    The following packages will be upgraded:
      ccache curl gdb libcurl4 libdw1 libelf1
    6 upgraded, 5 newly installed, 0 to remove and 906 not upgraded.
    Need to get 9,201 kB of archives.
    After this operation, 20.2 MB of additional disk space will be used.
    Get:1 http://kali.download/kali kali-rolling/main amd64 ccache amd64 4.1-1 [420 kB]
    Get:2 http://kali.download/kali kali-rolling/main amd64 curl amd64 7.72.0-1 [264 kB]
    Get:3 http://kali.download/kali kali-rolling/main amd64 libcurl4 amd64 7.72.0-1 [336 kB]
    Get:4 http://kali.download/kali kali-rolling/main amd64 libdw1 amd64 0.182-1 [234 kB]
    Get:5 http://kali.download/kali kali-rolling/main amd64 libelf1 amd64 0.182-1 [166 kB]
    Get:6 http://kali.download/kali kali-rolling/main amd64 libdebuginfod1 amd64 0.182-1 [25.4 kB]
    Get:7 http://kali.download/kali kali-rolling/main amd64 libpython3.9-minimal amd64 3.9.0-5 [797 kB]
    Get:8 http://kali.download/kali kali-rolling/main amd64 libnsl2 amd64 1.3.0-2 [39.5 kB]
    Get:9 http://kali.download/kali kali-rolling/main amd64 libpython3.9-stdlib amd64 3.9.0-5 [1,750 kB]
    Get:10 http://kali.download/kali kali-rolling/main amd64 libpython3.9 amd64 3.9.0-5 [1,688 kB]
    Get:11 http://kali.download/kali kali-rolling/main amd64 gdb amd64 10.1-1+b1 [3,481 kB]
    Fetched 9,201 kB in 3s (3,065 kB/s)
    (Reading database ... 263493 files and directories currently installed.)
    Preparing to unpack .../00-ccache_4.1-1_amd64.deb ...
    Unpacking ccache (4.1-1) over (3.7.11-1) ...
    Preparing to unpack .../01-curl_7.72.0-1_amd64.deb ...
    Unpacking curl (7.72.0-1) over (7.68.0-1+b1) ...
    Preparing to unpack .../02-libcurl4_7.72.0-1_amd64.deb ...
    Unpacking libcurl4:amd64 (7.72.0-1) over (7.68.0-1+b1) ...
    Preparing to unpack .../03-libdw1_0.182-1_amd64.deb ...
    Unpacking libdw1:amd64 (0.182-1) over (0.180-1+b1) ...
    Preparing to unpack .../04-libelf1_0.182-1_amd64.deb ...
    Unpacking libelf1:amd64 (0.182-1) over (0.180-1+b1) ...
    Selecting previously unselected package libdebuginfod1:amd64.
    Preparing to unpack .../05-libdebuginfod1_0.182-1_amd64.deb ...
    Unpacking libdebuginfod1:amd64 (0.182-1) ...
    Selecting previously unselected package libpython3.9-minimal:amd64.
    Preparing to unpack .../06-libpython3.9-minimal_3.9.0-5_amd64.deb ...
    Unpacking libpython3.9-minimal:amd64 (3.9.0-5) ...
    Selecting previously unselected package libnsl2:amd64.
    Preparing to unpack .../07-libnsl2_1.3.0-2_amd64.deb ...
    Unpacking libnsl2:amd64 (1.3.0-2) ...
    Selecting previously unselected package libpython3.9-stdlib:amd64.
    Preparing to unpack .../08-libpython3.9-stdlib_3.9.0-5_amd64.deb ...
    Unpacking libpython3.9-stdlib:amd64 (3.9.0-5) ...
    Selecting previously unselected package libpython3.9:amd64.
    Preparing to unpack .../09-libpython3.9_3.9.0-5_amd64.deb ...
    Unpacking libpython3.9:amd64 (3.9.0-5) ...
    Preparing to unpack .../10-gdb_10.1-1+b1_amd64.deb ...
    Unpacking gdb (10.1-1+b1) over (9.2-1) ...
    Setting up libpython3.9-minimal:amd64 (3.9.0-5) ...
    Setting up ccache (4.1-1) ...
    Updating symlinks in /usr/lib/ccache ...
    Setting up libnsl2:amd64 (1.3.0-2) ...
    Setting up libcurl4:amd64 (7.72.0-1) ...
    Setting up curl (7.72.0-1) ...
    Setting up libelf1:amd64 (0.182-1) ...
    Setting up libpython3.9-stdlib:amd64 (3.9.0-5) ...
    Setting up libdw1:amd64 (0.182-1) ...
    Setting up libdebuginfod1:amd64 (0.182-1) ...
    Setting up libpython3.9:amd64 (3.9.0-5) ...
    Setting up gdb (10.1-1+b1) ...
    Processing triggers for libc-bin (2.31-2) ...
    Processing triggers for man-db (2.9.3-2) ...
    Processing triggers for kali-menu (2020.3.2) ...


    [sudo] password for kali: 
    WARNING: apt does not have a stable CLI interface. Use with caution in scripts.
    
    dpkg-preconfigure: unable to re-open stdin: No such file or directory


## Install multilib for gcc and g++ 
- to be able to compile for 32 bit (x86) machine if using x64 (64 bit machine)


```bash
%%bash
# do this only if you have x64 Linux VM
echo kali | sudo -S apt install -y gcc-multilib g++-multilib
```

    Reading package lists...
    Building dependency tree...
    Reading state information...
    g++-multilib is already the newest version (4:10.2.0-1).
    gcc-multilib is already the newest version (4:10.2.0-1).
    The following packages were automatically installed and are no longer required:
      libcdio18 libmpdec2 libprotobuf22 libx264-155 libx264-159
    Use 'sudo apt autoremove' to remove them.
    0 upgraded, 0 newly installed, 0 to remove and 906 not upgraded.


    [sudo] password for kali: 
    WARNING: apt does not have a stable CLI interface. Use with caution in scripts.
    


## Compiling with gcc/g++ as 32-bit program in 64-bit system
- use -m32 switch with gcc and g++
```bash
gcc -m32 -o output input.c
g++ -m32 -o output input.cpp
```


```python
! cat ../demos/hello.cpp
```

    #include <iostream>
    
    using namespace std;
    
    int main()
    {
        cout << "Hello World!" << endl;
        return 0;
    }


```python
! g++ -m32 -o hello.exe ../demos/hello.cpp
```


```python
! g++ -m64 -o hello64.exe ../demos/hello.cpp
```

## Install and configure PEDA
- PEDA: Python Exploit Development Assistance for GDB
- https://github.com/longld/peda



```bash
%%bash
dirPath=~/peda 
if [ -d "$dirPath" ] # if peda directory exists in user's home directory
then
    echo "Directory $dirPath exists... Peda was already downloaded!";
    exit 0;
else
    git clone https://github.com/longld/peda.git $dirPath
    if [ $? == 0 ]; # if the last command successfuly executed
    then
        echo "source $dirPath/peda.py" >> ~/.gdbinit;
        echo "Finished downloading peda and configuring gdb!";
        exit 0;
    else
        echo "failed cloning peda"
        exit 1;
    fi
fi
```

    Cloning into '/home/kali/peda'...


    Finished downloading peda and configuring gdb!



```python

```
