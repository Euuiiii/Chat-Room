# Chat Application in C



## Overview
This project is my final submission for **CS50x**.
I built a simple **chat application** in C using **sockets** to handle network communication. The main goal was to create a functional chat room where multiple clients could connect and exchange messages in real time.

The project consists of two main programs:
- **Server** – Listens on a chosen port and manages all connected clients.
- **Client** – Connects to the server and allows a user to send and receive messages.

Whenever a client sends a message, the server broadcasts it to all other connected clients. This taught me a lot about handling multiple connections simultaneously and working with low-level network programming in C.

---

## Features
- Written entirely in **C**, which helped me strengthen my understanding of pointers, structs, and memory management.
- Uses **TCP sockets** for reliable communication.
- Supports multiple clients connecting at the same time.
- Broadcasts messages to all connected clients in real time.
- Handles client disconnections gracefully, avoiding crashes or memory leaks.
- Uses `select()` for multiplexing input/output, so the server can manage multiple clients efficiently.
- Comes with a simple terminal interface (with the option to extend using **ncurses** for a better UI).

---

## Files
- `server.c` → Starts the server, listens for incoming connections, and relays messages.
- `client.c` → Connects to the server and allows chatting from the terminal.
- `README.md` → Documentation for the project (this file).

---

## Design Choices
I debated a few design choices while building this project:

1. **TCP vs UDP** – I chose TCP for reliable delivery of messages. I wanted to make sure that no messages get lost, which is critical for chat applications.
2. **select() vs multithreading** – Instead of spawning a thread for each client, I used `select()` to manage multiple sockets in a single-threaded model. This simplified synchronization and avoided race conditions.
3. **Terminal interface** – I decided to start with a simple text-based interface first. Using **ncurses** can improve the UI later, but my focus was on network functionality.
4. **Message protocol** – Messages are plain text terminated by `\n`. I considered using a binary protocol, but keeping it human-readable made debugging much easier.

Building this project helped me learn the importance of handling partial reads/writes, client disconnections, and proper socket cleanup. I also gained practical experience with error handling and debugging in networked programs.

---

## How to Compile
You can compile both programs using `gcc`:

```bash
gcc server.c -o server
gcc client.c -o client
```

---

## How to Run

### Start the Server
Run the server on a chosen port (use a port number higher than 1024):

```bash
./server 8080
```

### Start the client
```bash
./client 127.0.0.1 8080
```

## Demo / Screenshots

### Server Running
![Server Logs](assets/server.png)

### Client Communication
| Client 1 | Client 2 |
| :---: | :---: |
| ![Client 1](assets/Client1.png) | ![Client 2](assets/Client2.png) |

### Personal Notes

While working on this project, I learned a lot about networking basics and C programming. Debugging multiple client connections was challenging at first, but seeing messages flow correctly between clients was very rewarding. This project gave me confidence to tackle more advanced networked applications in the future.


