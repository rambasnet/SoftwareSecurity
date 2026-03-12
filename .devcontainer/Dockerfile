FROM mcr.microsoft.com/devcontainers/base:ubuntu-22.04

# Install security tooling and multilib compilers needed by this workspace.
RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    build-essential \
    gcc-multilib \
    g++-multilib \
    gdb \
    gdb-multiarch \
    checksec \
    binutils \
    file \
    patchelf \
    strace \
    ltrace \
    tmux \
    ruby-full \
    wget \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Install pwndbg, a GDB plugin that provides enhanced debugging features for binary exploitation.
RUN curl --proto '=https' --tlsv1.2 -LsSf 'https://install.pwndbg.re' | sh -s -- -t pwndbg-gdb
