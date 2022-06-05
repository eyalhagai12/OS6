#include "Singleton.cpp"

#include <iostream>
#include <vector>
#include <poll.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

class Reactor
{
public:
    std::vector<pollfd> fds;
    std::vector<void (*)(int)> funcs;
    Reactor(/* args */);
    ~Reactor();
};

Reactor::Reactor(/* args */) : fds()
{
}

Reactor::~Reactor()
{
}

void *newReactor()
{
    return Singleton<Reactor>::Instance(new Reactor());
}

void installHandler(Reactor *reactor, void (*func)(int), int fd)
{
    pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN;
    reactor->fds.push_back(pfd);
    reactor->funcs.push_back(func);
}

void removeHandler(Reactor *reactor, int fd)
{
    for (size_t i = 0; i < reactor->fds.size(); ++i)
    {
        if (reactor->fds.at(i).fd == fd)
        {
            reactor->fds.erase(reactor->fds.begin() + i);
            reactor->funcs.erase(reactor->funcs.begin() + i);
        }
    }
    close(fd);
}

// --------------------------------------------------------------------------------------------------------------

#define PORT "3490" // the port users will be connecting to

#define BACKLOG 10 // how many pending connections queue will hold

int fd_count = 0;

// Get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

// Return a listening socket
int get_listener_socket(void)
{
    int listener; // Listening socket descriptor
    int yes = 1;  // For setsockopt() SO_REUSEADDR, below
    int rv;

    struct addrinfo hints, *ai, *p;

    // Get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0)
    {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }

    for (p = ai; p != NULL; p = p->ai_next)
    {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0)
        {
            continue;
        }

        // Lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
        {
            close(listener);
            continue;
        }

        break;
    }

    freeaddrinfo(ai); // All done with this

    // If we got here, it means we didn't get bound
    if (p == NULL)
    {
        return -1;
    }

    // Listen
    if (listen(listener, 10) == -1)
    {
        return -1;
    }

    return listener;
}

void handle_client(int fd)
{
    char buf[256]; // Buffer for client data
    Reactor *reactor = Singleton<Reactor>::Instance()->get_data();

    // If not the listener, we're just a regular client
    int nbytes = recv(fd, buf, sizeof buf, 0);

    int sender_fd = fd;

    if (nbytes <= 0)
    {
        // Got error or connection closed by client
        if (nbytes == 0)
        {
            // Connection closed
            printf("pollserver: socket %d hung up\n", sender_fd);
        }
        else
        {
            perror("recv");
        }

        close(fd); // Bye!

        removeHandler(reactor, fd);
        fd_count--;
    }
    else
    {
        // We got some good data from a client

        for (int j = 0; j < fd_count; j++)
        {
            // Send to everyone!
            int dest_fd = reactor->fds.at(j).fd;

            // Except the listener and ourselves
            if (dest_fd != reactor->fds.at(0).fd && dest_fd != sender_fd)
            {
                if (send(dest_fd, buf, nbytes, 0) == -1)
                {
                    perror("send");
                }
            }
            else if (dest_fd == sender_fd)
            {
                if (send(dest_fd, "message was delivered\n", 23, 0) == -1)
                {
                    perror("send");
                }
            }
            
        }
    }
}

void accept_client(int fd)
{
    // If listener is ready to read, handle new connection

    struct sockaddr_storage remoteaddr;
    socklen_t addrlen = sizeof remoteaddr;
    int newfd = accept(fd,
                       (struct sockaddr *)&remoteaddr,
                       &addrlen);
    char remoteIP[INET6_ADDRSTRLEN];

    if (newfd == -1)
    {
        perror("accept");
    }
    else
    {
        installHandler(Singleton<Reactor>::Instance()->get_data(), &handle_client, newfd);
        fd_count++;

        printf("pollserver: new connection from %s on "
               "socket %d\n",
               inet_ntop(remoteaddr.ss_family,
                         get_in_addr((struct sockaddr *)&remoteaddr),
                         remoteIP, INET6_ADDRSTRLEN),
               newfd);
    }
}

// Main
int main(void)
{
    int listener; // Listening socket descriptor

    int newfd;                          // Newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // Client address
    socklen_t addrlen;

    char buf[256]; // Buffer for client data

    char remoteIP[INET6_ADDRSTRLEN];

    // Start off with room for 5 connections
    // (We'll realloc as necessary)
    Singleton<Reactor> *sing = Singleton<Reactor>::Instance();
    Reactor *reactor = sing->get_data();
    
    // Set up and get a listening socket
    listener = get_listener_socket();

    if (listener == -1)
    {
        fprintf(stderr, "error getting listening socket\n");
        exit(1);
    }

    // Add the listener to set
    installHandler(reactor, &accept_client, listener);
    // pfds[0].fd = listener;
    // pfds[0].events = POLLIN; // Report ready to read on incoming connection

    fd_count = 1; // For the listener

    // Main loop
    for (;;)
    {
        int poll_count = poll(&(reactor->fds.at(0)), fd_count, -1);

        if (poll_count == -1)
        {
            perror("poll");
            exit(1);
        }

        // Run through the existing connections looking for data to read
        for (int i = 0; i < fd_count; i++)
        {

            // Check if someone's ready to read
            if (reactor->fds.at(i).revents & POLLIN)
            { // We got one!!

                if (reactor->fds.at(i).fd == listener)
                {
                    reactor->funcs.at(i)(reactor->fds.at(i).fd);
                }
                else
                {
                    reactor->funcs.at(i)(reactor->fds.at(i).fd);
                }
            } // END handle data from client
        }     // END got ready-to-read from poll()
    }         // END looping through file descriptors
              // END for(;;)--and you thought it would never end!
    return 0;
}
