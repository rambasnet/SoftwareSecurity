# Use Ubuntu 22.04 image
FROM ubuntu:22.04

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Update packages and install gdb, make, and g++ in one step
RUN apt update && \
    apt install -y net-tools gdb build-essential && \
    apt install -y gcc-multilib g++-multilib && \
    apt clean && \
    rm -rf /var/lib/apt/lists/*

# Add the secret flag to a file
RUN echo "flag{!Softwaresecurity is fun!}" > /secret.txt

# Set permissions (not really secure in Docker, but still)
RUN chmod 400 /secret.txt

# Set working directory
WORKDIR /var/www

# Expose your desired port
EXPOSE 80

# Default command
CMD ["/bin/bash"]

