# TCP-multiplexed-server

Description
-----------

This is a small non-blocking TCP chat server in C.
It listens on `127.0.0.1` and allows multiple clients to communicate with each other using socket multiplexing via `select()`.

Features
--------

*   TCP server bound to `127.0.0.1`
    
*   Handles multiple clients simultaneously
    
*   Non-blocking I/O using `select()`
    
*   Unique incremental ID assigned to each client
    
*   Broadcasts messages between clients
    
*   Proper connection and disconnection notifications
    
*   No memory or file descriptor leaks
    

Usage
-----

### Compilation: 
    gcc -Wall -Wextra -Werror tcp_server.c -o tcp_server

### Execution: 
    ./tcp_server <port>

### If no port is provided, the program exits with:
    Wrong number of arguments

Testing
-------

You can test the server using `nc` (netcat):

`nc 127.0.0.1 8080`

Open multiple terminals to simulate multiple clients.

Server Behavior
---------------

### Client Connection

*   Each client receives a unique ID starting from `0`
    
*   When a client connects, all existing clients receive:    

    `server: client <id> just arrived`
    

### Message Handling

*   Clients can send printable characters
    
*   Each line is broadcasted to other clients with the format:

    `client <id>: <message>`
    

### Client Disconnection

*   When a client disconnects, remaining clients receive:

    `server: client <id> just left`

Allowed Functions
-----------------

`write, close, select, socket, accept, listen, send, recv, bind, strstr, malloc, realloc, free, calloc, bzero, atoi, sprintf, strlen, exit, strcpy, strcat, memset`

* * *

Project Files
-------------

```
    .
    ├── tcp_server.c 
    └── README.md`
```