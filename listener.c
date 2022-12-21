//datagram sockets "server" demo

#include <stdio.h>  //fprintf
#include <stdlib.h> //exit(0)
#include <unistd.h> // is this really needed?
#include <errno.h> //int errno, holds error information for system calls
#include <sys/types.h> //bind, socket, getaddrinfo 
#include <sys/socket.h> //socket, getaddrinfo, bind, connect, accept, 
#include <netinet/in.h> //inet_addr structs, 
#include <arpa/inet.h> //inet_ntop -> convert ip addr to human readable form
#include <netdb.h> // getaddrinfo, gethostbyname, gethostbyaddr, getnameinfo()

// #include <unistd.h> //only linux
// #include <string.h>
// #include <sys/types.h>
// #include <sys/types.h>

#define MYPORT "4950"
#define MAXBUFLEN 100

void *get_in_addr(struct sockaddr *sa) { //get ip address depending on version
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void) {
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    socklen_t addr_len; //opque type of at least 32 bits
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; //
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "get ddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = servinfo; p != NULL; p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("listener, bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket");
        return 2;
    }

    freeaddrinfo(servinfo);

    addr_len = sizeof their_addr;

    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN - 1, 0, 
        (struct sockaddr *)&their_addr, &addr_len))) {
            perror("recvfrom");
            exit(1);
    }

    printf("got packet from %s\n", inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s));

    printf("listener is %d bytes long \n", numbytes);

    buf[numbytes] = '\0';
    printf("listener: packet contains \"%s\"\n", buf);

    close(sockfd);

    return 0;   

    // if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN - 1, 0, )))
}