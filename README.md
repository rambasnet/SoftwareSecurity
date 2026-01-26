# System and Software Security and Exploitation

- Fundamentals of System and C/C++ Software Security and Exploitation
- Hacking Tricks, Techniques & Tools
- System software flaws and exploitations
- Primary Tools: C/C++, Bash, Python3
- Capture the Flag (CTF) challenges
- Vulnerability discovery and exploitation

## Requirements

- Downlaod Kali Linux 64-bit for your system: https://www.kali.org/downloads/
- Install Kali Linux on a Virtual Machine (VM) or on a physical machine
- Install VirtualBox or VMware Workstation Player
- Install VirtualBox Guest Additions or VMware Tools
- Install the following tools and packages on Kali Linux:
  - add account kali:kali with sudo access
- gcc/g++ compilers
- make
- Jupyter Notebook
- Python3
- gdb
- peda - python exploit development assistant for gdb
- sqlite3 C/C++ library
- sqlitebrowser
- pwntools
- Docker and docker-compose

## Install Required Tools and Packages

- update Linux and install tools
- run the following commands on a Terminal

```bash
sudo apt update
sudo apt upgrade
sudo apt install build-essential # C/C++ compiler, make, etc.
sudo apt install ccache
sudo apt install libboost-all-dev
sudo apt install gcc-multilib g++-multilib # library for compiling 32-bit; won't work on Mac M1/M2
sudo apt install gdb
sudo apt install gdbserver
sudo apt install git
sudo apt install libsqlite3-dev # C++ library for Sqlite
sudo apt install sqlitebrowser
sudo apt install python3-pip
sudo apt install python3-pwntools
```

## Install Jupyter Notebook

- install Jupyter Notebook using pip3

```bash
python3 -m pip install notebook
```

## Create Snapshot

- create a snapshot of the VM after installing all the tools and packages
- this will allow you to revert back to this state if needed
- this is important for the labs and exercises in this course
- you can create a snapshot by going to the VirtualBox or VMware Workstation Player menu and selecting "Take Snapshot"
- name the snapshot something like "Initial Setup" or "Before Labs"

## Play with Notebooks

- clone/download this repository
- compile and run C++ programs found in `demos` folder
- using a Terminal `cd` into the repository folder and run `jupyter notebook`

```bash
git clone https://github.com/rambasnet/SoftwareSecurity.git
cd SoftwareSecurity
jupyter notebook
```

- copy and paste the link shown on a browser if it doesn't open on its own
- open `notebooks/00-TableOfContents.ipynb` notebook that provides table of contents with link to all the notebooks
