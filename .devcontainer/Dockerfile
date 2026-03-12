FROM mcr.microsoft.com/devcontainers/base:ubuntu-22.04

# Install security tooling and multilib compilers needed by this workspace.
RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    build-essential \
    gcc-multilib \
    g++-multilib \
    git \
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
    netcat-openbsd \
    python-is-python3 \
    && rm -rf /var/lib/apt/lists/*

# Make sudo python/python3 resolve to the same 3.11 toolchain path used by the devcontainer feature.
RUN ln -sf /usr/local/python/current/bin/python /usr/local/bin/python \
    && ln -sf /usr/local/python/current/bin/python3 /usr/local/bin/python3

# Install pwndbg, a GDB plugin that provides enhanced debugging features for binary exploitation.
RUN curl --proto '=https' --tlsv1.2 -LsSf 'https://install.pwndbg.re' | sh -s -- -t pwndbg-gdb

# Install PEDA and provide a dedicated launcher to avoid conflicting with pwndbg defaults.
RUN git clone --depth 1 https://github.com/longld/peda.git /opt/peda \
    && chmod -R a+rX /opt/peda \
    && printf '%s\n' '#!/usr/bin/env bash' 'exec gdb -q -ex "source /opt/peda/peda.py" "$@"' > /usr/local/bin/gdb-peda \
    && chmod +x /usr/local/bin/gdb-peda
