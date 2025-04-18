# Use Ubuntu 22:04 image
FROM ubuntu:22.04

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Update packages and install gdb, make, and g++ in one step
RUN apt update && \
    apt install -y net-tools gdb build-essential && \
    apt install -y libc6-dev gcc-multilib g++-multilib && \
    apt clean && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /var/www

# Add the secret flag to a file
RUN echo "flag{!Congratulations... You did it!}" > /confidential.txt

# Set permissions (not really secure in Docker, but still)
RUN chmod 400 /confidential.txt

# Expose your desired port
EXPOSE 80

# Default command (you can change this)
CMD ["/bin/bash"]

