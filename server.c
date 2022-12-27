// struct addrinfo {
// int ai_flags; // AI_PASSIVE, AI_CANONNAME, etc.
// int ai_family; // AF_INET, AF_INET6, AF_UNSPEC
// int ai_socktype; // SOCK_STREAM, SOCK_DGRAM
// int ai_protocol; // use 0 for "any"
// size_t ai_addrlen; // size of ai_addr in bytes
// struct sockaddr *ai_addr; // struct sockaddr_in or _in6
// char *ai_canonname; // full canonical hostname
// struct addrinfo *ai_next; // linked list, next node
// };

// stream socket server demo

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>

#define PORT "3490"
#define BACKLOG 10

void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void) {
    int sockfd;
    int new_fd;
    struct addrinfo hints, *servinfo, *p;
    socklen_t sin_size; //unsigned opaque integer
    struct sockaddr_storage their_addr;
    int status;
    int yes = 1;
	struct sigaction sa;
    char s[INET6_ADDRSTRLEN];


    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    for (p = servinfo; p != NULL; p->ai_next) {
        if (sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
            close(sockfd);
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        return 2;
    }

    freeaddrinfo(servinfo);

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

   	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	} 

    printf("server: waiting for connections...\n");

    while(1) {
        //testing
        sin_size = sizeof(their_addr);
        new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &sin_size);

        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr*)&their_addr), s, sizeof s);
        printf("server: connection from %s\n", s);

        if (!fork()) {
            close(sockfd);
            if (send(new_fd, "hi", 13, 0) == -1) {
                perror('send');
            }

            close(new_fd);
            exit(0);
        }

        close(new_fd);
    }

    return 0;
}

