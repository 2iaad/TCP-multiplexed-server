#include <unistd.h> // for write, close, read.
#include <string.h> // for strlen, bzero.
#include <stdlib.h> // for exit, atoi.
#include <stdio.h> // for sprintf.
#include <netinet/ip.h> // for sockaddr_in.
#include <sys/select.h> // for select and fd_set.

/*
	id: unique client ID assigned when they connect.
	msg: a buffer to store messages from the client (large enough to hold many characters).
*/
typedef struct	s_client{
	int		id;
	char	msg[424242];
}	t_client;

t_client	clients[1024];

int			max = 0, next_id = 0;
char		buffRead[424242], buffWrite[424242];
fd_set		Readfds, Writefds, curr; // fd_set: data structure used by select(), a list of file descriptors I want the OS to watch

void	printError(char *str)
{
	write(2, str, strlen(str));
	exit(1);
}

void	msgSender(int sendfd)
{
	for (int fd = 0; fd <= max ; fd++)
	{
		if (FD_ISSET(fd, &Writefds) && fd != sendfd)
			write(fd, buffWrite, strlen(buffWrite));
	}
}

int	main(int ac,char** av)
{
	if (ac != 2)
        printError("Wrong number of arguments\n");

	int				    socketFd;
	struct sockaddr_in  servaddr; // pre-defined data structure that holds infos about a network adress (version, ip, port.. etc)

    /*
        socket() : create endpoint for communication

        AF_INET: It tells the OS that the socket will use IPv4 addresses
        SOCK_STREAM: I want to use reliable, two-way connection based byte streams.
        IPPROTO_TCP || 0: protocol to use.
    */
	socketFd = max = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketFd == -1)
        printError("Fatal error\n");

    // init the data structure to empty to remove garbage values
	FD_ZERO(&curr);
    // Adds socketFd to the set curr, telling the os to keep track of actions on socketFd
	FD_SET(socketFd, &curr);

    /*
        specify infos about the address we are going to link with our socketFd
        htonl: Some computers store least-significant byte first, some most-significant byte first
    */
	servaddr.sin_family = AF_INET; // This address uses IPv4
	servaddr.sin_addr.s_addr = htonl(2130706433); // 127.0.0.1 written as one 32-bit number.
	servaddr.sin_port = htons(atoi(av[1])); // port to listen to

	if ((bind(socketFd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) // link socket with adress
		printError("Fatal error\n");
    // starting the server with listen
	if (listen(socketFd, 10) != 0) // backlog: 10 people can wait in queue to connect
        printError("Fatal error\n");

    /*
        The loop is "hanged" inside select() waiting for incoming events such as:
            -> A new client trying to connect (listening socket becomes readable)
            -> An existing client sending data (client socket becomes readable)

        * When an event happens, select() returns
        * Readfds/Writefds are updated by select() to contain only the fds that are ready (thats why we init them each time)
    */
	while(1)
	{
		Readfds = Writefds = curr;
		if (select(max + 1, &Readfds, &Writefds, NULL, NULL) < 0) // listen for events | (max + 1) because select listens to (max-1) RTFM
            continue;
		for (int fd = 0; fd <= max; fd++)
		{
            // accepting new client connections
			if (FD_ISSET(fd, &Readfds) && fd == socketFd) // if fd is marked as ready for read + fd = socketFd => newClient
            {
				int newClient = accept(socketFd, NULL, NULL);
				if (newClient > max)
					max = newClient;
				clients[newClient].id = next_id++;
				bzero(clients[newClient].msg, sizeof(clients[newClient].msg)); // Clears the message buffer for this client.
				FD_SET(newClient, &curr); // add new client to tracked sockets to stay notified about events
				sprintf(buffWrite, "server: client %d just arrived\n", clients[newClient].id);
				msgSender(newClient);
				break;
			}
            
			if (FD_ISSET(fd, &Readfds) && fd != socketFd)
			{
				int read = recv(fd, buffRead, sizeof(buffRead), 0);
				if(read <= 0)
				{
					sprintf(buffWrite, "server: client %d just left\n", clients[fd].id);
					msgSender(fd);
					FD_CLR(fd, &curr);
					close(fd);
					break;
				}
				else
				{
					for(int i = 0, j = strlen(clients[fd].msg); i < read; i++, j++)
					{
						clients[fd].msg[j] = buffRead[i];
						if(clients[fd].msg[j] == '\n')
						{
							clients[fd].msg[j] = '\0';
							sprintf(buffWrite, "client %d: %s\n", clients[fd].id, clients[fd].msg);
							msgSender(fd);
							bzero(clients[fd].msg, strlen(clients[fd].msg));
							j = -1;
						}
					}
					break;
				}
			}
		}
	}
}