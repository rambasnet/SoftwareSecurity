# System and Software Security and Exploitation

- Fundamentals of System and C/C++ Software Security and Exploitation
- Hacking Tricks, Techniques & Tools
- Primary Tools: C++, Bash, Python3
- CTF

# Requirements

- Ubuntu/Debian Linux (Kali Linux 64-bit Preferred)
  - add account kali:kali with sudo access
- gcc/g++
- Jupyter Notebook
- Python3
- gdb
- peda - python exploit development assistant for gdb
- sqlite3 C/C++ library
- sqlitebrowser
- pwntools
- boost C++ libraries

### On a local VM system

- must use Linux x86-64-bit system
To run these notebooks interactively and save your work locally, you need [Python 3](https://www.python.org/) and [Jupyter Notebook](http://jupyter.org/) -- an interactive web-based editor that allows you to create and share documents that contain live code and data. [Anaconda or Miniconda](https://www.anaconda.com/products/distribution) is the recommended way to install Python and other packages on all modern platforms.

#### Using Docker

- Install Docker on your system: [https://docs.docker.com/get-docker/](https://docs.docker.com/get-docker/)

- Dockerfile is provided to build a container image with all the necessary packages and dependencies to run the notebooks. You can build the image and run the container using the provided bash scripts.

- Clone this repo and run the following command from the root of the repo
- Use git-bash Terminal on Windows to run bash scripts

```bash
    git clone https://github.com/rambasnet/Python-Object-Oriented-Programming.git
    bash run-jupyter.sh # run jupyter notebook server in the container
```

- Jupiter Notebooks are inside the `notebooks` folder
- start from the `notebbooks/OO-Table-of-Contents.ipynb`


# Install Required Tools and Packages

- update Linux and install tools
- install Python 3.x Miniconda for Linux: https://conda.io/en/latest/miniconda.html
- run the following commands on a Terminal

```bash
sudo apt update
sudo apt upgrade
sudo apt install build-essential # C/C++ compiler, make, etc.
sudo apt install ccache
sudo apt install libboost-all-dev
sudo apt install gcc-multilib g++-multilib # library for compiling 32-bit
sudo apt install gdb
sudo apt install gdbserver
sudo apt install git
sudo apt install libsqlite3-dev # C++ library for Sqlite
sudo apt install sqlitebrowser
curl -o Miniconda.sh https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh
bash Miniconda.sh # pick defaults; python 3 is installed!
conda update conda
conda install jupyter notebook # jupyter notebook
conda install -c conda-forge xeus-cling # C++ kernel
conda install -c conda-forge retrolab # replate classic notebook with retro style
pip install ptpython # better python REPL for terminal
```

# Play with Notebooks

- clone/download this repository
- compile and run C++ programs found in `demos` folder
- using a Terminal `cd` into the repository folder and run `jupyter notebook`

```bash
git clone https://github.com/rambasnet/SoftwareSecurity.git
cd SoftwareSecurity
jupyter retro
```

- copy and paste the link shown on a browser if it doesn't open on its own
- open notebooks/00-TableOfContents.ipynb notebook that provides table of contents with link to all the notebooks
