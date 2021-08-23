# Ethical Hacking

- fundamentals of System & Network Exploitation
- collection of hacking tricks, techniques & tools
- primary tools: C++, g++, Bash, Python3

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

# Install Required Tools
- update Linux and install tools
- install Python 3.x Miniconda for Linux: https://conda.io/en/latest/miniconda.html
- run the following commands on a Terminal

```bash
sudo apt update
sudo apt upgrade
sudo apt install build-essential
sudo apt install ccache
sudo apt install libboost-all-dev
sudo apt install gcc-multilib g++-multilib
sudo apt install gdb
sudo apt install gdbserver
sudo apt install git
sudo apt install libsqlite3-dev
sudo apt install sqlitebrowswer
curl -o Miniconda.sh https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh
bash Miniconda.sh # pick defaults; python 3 is installed!
conda update conda
conda install notebook # jupyter notebook
conda install -c conda-forge xeus-cling #c++ kernel
pip install ptpyton # better python REPL for terminal
```

# Play with Notebooks

- clone/download this repository
- compile and run C++ programs found in `demos` folder
- using a terminal `cd` into the repository folder and run `jupyter notebook`

```bash
cd <EthicalHacking_Repo>
jupyter notebook
```

- open the link shown in a browser if it's not opened on its own
- open 00-TableOfContents.ipynb chapter that provides table of contents with link to all the notebooks
