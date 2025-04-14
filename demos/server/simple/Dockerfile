# Use Ubuntu 22:04 image
FROM ubuntu:22.04

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Update packages and install make and g++
RUN apt update && \
    apt install -y net-tools && \
    apt install -y build-essential libc6-dev gcc-multilib g++-multilib && \
    apt clean && \
    rm -rf /var/lib/apt/lists/*

# Expose your desired port
EXPOSE 7890

WORKDIR /app

# Default command (you can change this)
CMD ["/bin/bash"]

