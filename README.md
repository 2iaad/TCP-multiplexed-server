# TCP-multiplexed-server

Description
-----------

This is a small non-blocking TCP chat server in C.
It listens on `127.0.0.1` and allows multiple clients to communicate with each other using socket multiplexing via `select()`.
![src](https://ops.tips/blog/-/images/tcp-connection-overview.svg)

Features
--------

*   TCP server bound to `127.0.0.1`
    
*   Handles multiple clients simultaneously
    
*   Non-blocking I/O using `select()`
    
*   Broadcasts messages between clients
    
*   Proper connection and disconnection notifications

Server Event Loop (Flow Diagram)
-------------
```
                +--------------------+
                |   Start Server     |
                +--------------------+
                          |
                          v
                +--------------------+
                | socket / bind /    |
                | listen             |
                +--------------------+
                          |
                          v
                +--------------------+
                |   Main Loop        |
                |    select()        |
                +--------------------+
                    /              \
                   /                \
        +----------------+      +--------------------+
        | New connection |      | Client socket ready|
        | (listen fd)    |      | (recv/send)        |
        +----------------+      +--------------------+
                |                             |
                v                             v
      +--------------------+   +----------------------+
      | accept() client    |   | recv() message       |
      | assign ID          |   | or detect disconnect |
      +--------------------+   +----------------------+
                |                             |
                v                             v
  +-----------------------------+   +-----------------------------+
  | Notify other clients:       |   | If message received:        |
  | "client X just arrived"     |   | broadcast to other clients  |
  +-----------------------------+   +-----------------------------+
                                              |
                                              v
                                    +-----------------------------+
                                    | If disconnected:            |
                                    | close fd, free memory,      |
                                    | notify clients              |
                                    +-----------------------------+
```

Testing
-------

### Compilation && Execution: 
    gcc -Wall -Wextra -Werror tcp_server.c -o tcp_server && ./tcp_server <port>

You can test the server using `nc` (netcat):

`nc 127.0.0.1 8080`

Open multiple terminals to simulate multiple clients.

Functions i used
-----------------

`accept(), atoi(), bind(), close(), exit(), listen(), memcpy(), memset(), recv(), select(), socket(), strlen(), write().`

