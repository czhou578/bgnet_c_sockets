#include <stdio.h>  //fprintf
#include <stdlib.h> //exit(0)
#include <unistd.h> //only linux
#include <errno.h> 
// #include <string.h>
// #include <netdb.h>
// #include <sys/types.h>
#include <sys/socket.h> //includes AF_INET
// #include <sys/types.h>

#define PORT = "3490"
#define MAXDATASIZE 100

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    
    return &(((struct sockaddr_in6*)sa)->sin6_addr);


}

int main() {
    // printf("hello %s", "string");



    return 0;
}