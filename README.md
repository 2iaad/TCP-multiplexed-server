# TCP-multiplexed-server

Description
-----------

This is a small non-blocking TCP chat server in C.
It listens on `127.0.0.1` and allows multiple clients to communicate with each other using socket multiplexing via `select()`.
![src](https://ops.tips/blog/-/images/tcp-connection-overview.svg)

The goal of this project:
-------------------------

*   Let multiple clients connect to my server on 127.0.0.1 (localhost) on a specific port.

*   Give each client a unique ID.

*   Allow clients to send messages to the server.

*   The server broadcast messages to all other clients in real time.

*   Inform clients when someone arrives or leaves.

*   Handle multiple clients without using threads (via `select()`).


Things i have learned:
----------------------

*   Low-level network programming in C (sockets, TCP/IP).

*   Event-driven programming using select() instead of threads.

*   Buffering and handling partial messages (clients can send messages in chunks).

*   Non-blocking I/O and why monitoring read/write readiness is important.

*   Chat apps, game servers, or real-time services work behind the scences.


Server Event Loop (Flow Diagram)
-------------
```
                +--------------------+
                |   socket / bind /  |
                |       listen       |
                +--------------------+
                          |
                          v
                +--------------------+
                |    Start Server    |
                +--------------------+
                          |
                          v
                +--------------------+
                |      Main Loop     |
                |       select()     |
                +--------------------+
                    /              \
                   /                \
        +----------------+       +--------------------+
        | New connection |       | Client socket ready|
        | (listen fd)    |       | (recv/send)        |
        +----------------+       +--------------------+
                |                             |
                v                             v
      +--------------------+     +----------------------+
      | accept() client    |     | recv() message       |
      | assign ID          |     | or detect disconnect |
      +--------------------+     +----------------------+
                |                             |
                v                             v
+-----------------------------+  +-----------------------------+
| Notify other clients:       |  | If message received:        |
| "client X just arrived"     |  | broadcast to other clients  |
+-----------------------------+  +-----------------------------+
                                              |
                                              v
                                  +-----------------------------+
                                  |  If disconnected:           |
                                  |   - close fd, free memory,  |
                                  |   - notify clients          |
                                  +-----------------------------+
```

Testing
-------

### Compilation && Execution: 
    cc -Wall -Wextra -Werror tcp_server.c -o tcp_server && ./tcp_server <port>

You can open a client on the server using `nc`(netcat):

`nc 127.0.0.1 8080`

Open multiple terminals to simulate multiple clients.

Now type a message from a client — it gets broadcasted to the other clients, enjoy ;).

Functions i used
-----------------

`accept(), atoi(), bind(), close(), exit(), listen(), memcpy(), memset(), recv(), select(), socket(), strlen(), write().`

