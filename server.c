#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#define PORT 3490
#define BACKLOG 10 /* how many pending connections queue will hold */


int socket(int domain, int type, int protocol);
int bind(int sockfd, struct sockaddr* myaddr, int addrlen);
int listen(int sockfd, int backlog);
int accept(int sockfd, struct sockaddr* cliaddr, int* addrlen);
int connect(int sockfd, struct sockaddr* servaddr, int addrlen);

main() {
    int sockfd, new_fd;             /* Listen on sock_fd, new connection on new_fd */
    struct sockaddr_in my_addr;     /* My address */
    struct sockaddr_in their_addr;  /* Connector address */
    int sin_size;

    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT); /* short, network byte order */
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* INADDR_ANY allows clinets to connet to any one of the host's IP address */

    if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }
    
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    while (1) { /* main accept() loop */
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accpet(sockfd, (struct sockaddr*) &their_addr, &sin_size)) == -1) {
            perror("accept");
            continue;
        }
        printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr));
    }
}

/**
 * @brief Create a socket
 * 
 * @param domain : protocol family -> "PF_INET" for IPv4 (typicall used)
 * @param type : style of communication -> "SOCK_STREAM" for TCP (with "PF_INET")
 * @param protocol : protocol within family -> Typically set to 0
 * @return file descriptor or -1. Also sets errno on failure.
 */
int socket(int domain, int type, int protocol) {

}


/**
 * @brief Bind a socket to a local IP address and port number
 * 
 * @param sockfd : socket file descripteor (returned from socket())
 * @param myaddr : includes IP address and port number
 *      - IP address : set by kernel if value passed is INADDR_ANY, else set by caller
 *      - port number : set by kernel if value passed is 0, else set by caller
 * @param addrlen : length of address structure -> sizeof(struct sockaddr_in)
 * @return int 0 on success, -1 and sets errno on failure
 */
int bind(int sockfd, struct sockaddr* myaddr, int addrlen) {

}


/**
 * @brief Put socket into passive state (wait for connections rather than initiate a connection).
 *        Listen() is non-blocking: returns immediately
 * 
 * @param sockfd : socket file descriptor (returned from socket())
 * @param backlog : bound on length of unaccepted connection queue (connection backlog); kernel will cap, thus better to set high
 * @return int 0 on success, -1 and sets errno on failure
 */
int listen(int sockfd, int backlog) {

}


/**
 * @brief Accept a new connection
 *        Accpet is blocking: Waits for connection before returning
 * 
 * @param sockfd : socket file descriptor (returned from socket())
 * @param cliaddr : IP address and port number of client (returned from call)
 * @param addrlen : length of address structure = pointer to int set to sizeof (struct sockaddr_in)
 * @return int file descriptor or -1. Also sets errno on failure.
 */
int accept(int sockfd, struct sockaddr* cliaddr, int* addrlen) {

}

/**
 * @brief Connect to another socket.
 *        Connect is blocking.
 * 
 * @param sockfd : socket file descriptor (returned from socket())
 * @param servaddr : IP address and port number of server
 * @param addrlen : length of address structure -> sizeof(struct sockaddr_in)
 * @return int 0 on success, -1 and sets errno on failure.
 */
int connect(int sockfd, struct sockaddr* servaddr, int addrlen) {

}