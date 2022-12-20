#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "3490"
#define BACKLOG 10

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void) {
    int sockfd;
    struct addrinfo *hints, *servinfo, *p;
    socklen_t sin_size;
    int status;

    memset(&hints, 0, sizeof hints);

    hints->ai_family = AF_UNSPEC;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    for (p = servinfo; p != NULL; p->ai_next) {
        if (sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol) == -1) {
            perror("server: socket");
            continue;
        }

        
    }

}

