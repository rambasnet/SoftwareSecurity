# Log Server

This lab contains a deliberately insecure TCP log server that listens on port `200` and echoes back whatever the client sends.

## Run with Docker Compose

From this directory, start the lab container:

```bash
docker compose up --build -d
```

The compose service keeps the container running but does not start the server binary automatically. Update and use Make to compile and run it inside the container:

```bash
docker compose exec server bash
make
./log_server.exe
```

When the server starts, it prints:

```text
Accepting requests on port 200
```

## Access the Server

The container publishes port `200` to the host, so you can connect from the host machine at `localhost:200`.

Send test input with netcat:

```bash
printf 'hello from client\n' | nc localhost 200
```

Or open an interactive session:

```bash
nc localhost 200
```

If you are connecting from another machine on the same network, replace `localhost` with the Docker host IP address and keep port `200`.

## Run Without Docker

You can also build and run the lab directly on Linux if 32-bit build support is installed:

```bash
g++ -c -g -Wall -std=c++17 -m32 main.cpp
g++ -m32 -fno-stack-protector -z execstack -no-pie -o log_server.exe main.o
./log_server.exe
```

Then connect from another terminal:

```bash
printf 'hello from client\n' | nc localhost 200
```

## Stop the Lab

If you started the environment with Docker Compose:

```bash
docker compose down
```
