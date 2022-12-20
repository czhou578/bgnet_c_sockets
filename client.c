#include <stdio.h>  //fprintf
#include <stdlib.h> //exit(0)
// #include <unistd.h> //only linux
#include <errno.h> 
// #include <string.h>
#include <netdb.h>
// #include <sys/types.h>
#include <sys/socket.h> //includes AF_INET
#include <arpa/inet.h> // for the inet_ntop 
// #include <sys/types.h>

#define PORT "3490"
#define MAXDATASIZE 100

void *get_in_addr(struct sockaddr *sa) //get address, ipv4 or ipv6
{
	if (sa->sa_family == AF_INET) { //if IPv4
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr); //for ipv6
}

int main(int argc, char *argv[]) {
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr, "usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints); //make sure struct is empty
    hints.ai_family = AF_UNSPEC; //either ipv4 or ipv6 is ok
    hints.ai_socktype = SOCK_STREAM; //we accept sock_stream

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct socaddr *)p->ai_addr), s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo);

    if ((numbytes = recv(sockfd, buf, MAXDATASIZE - 1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';

    printf("client: received '%s'\n", buf);

    close(sockfd);

    return 0;
}