//datagram client demo

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "4500"

int main(int argc, char*argv[]) {
    int sockfd;
    struct addrinfo hints, *results, *p;
    int rv;
    int numbytes;

    if (argc != 3) {
        fprintf(stderr, "adjhflakjhdf");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &results)) != 0) {
        fprintf(stderr, "getaddr info: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = results; p != NULL; p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "talker: failed to bind socket");
        return 2;
    }

    if ((numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0, p->ai_addr, p->ai_addrlen))) {
        perror("talker: sendto");
        exit(1);
    }

    freeaddrinfo(results);

    printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
	close(sockfd);

    return 0;

}