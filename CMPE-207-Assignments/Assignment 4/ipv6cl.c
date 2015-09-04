#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>


#include <netdb.h>

void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {
    int sock_fd, port_no, n;
    struct sockaddr_in6 serv_addr1;
    struct hostent *serv;
    char buff[256]="This”is client message";
port_no=13;
    

    printf("\nIPv6 TCP Client Started...\n");
    
    
    sock_fd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sock_fd < 0)
        error("ERROR opening socket");

  
    serv = gethostbyname2(argv[1],AF_INET6);
    if (serv == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    memset((char *) &serv_addr1, 0, sizeof(serv_addr1));
    serv_addr1.sin6_flowinfo = 0;
    serv_addr1.sin6_family = AF_INET6;
    memmove((char *) &serv_addr1.sin6_addr.s6_addr, (char *) serv->h_addr, serv->h_length);
    serv_addr1.sin6_port = htons(port_no);

   
    if (connect(sock_fd, (struct sockaddr *) &serv_addr1, sizeof(serv_addr1)) < 0)
        error("ERROR connecting");

  
    n = send(sock_fd,buff, strlen(buff)+1, 0);
    if (n < 0)
        error("ERROR writing to socket");

    memset(buff, 0, 256);
    
   
    n = recv(sock_fd, buff, 255, 0);
    if (n < 0)
        error("ERROR reading from socket");
    printf("Message from serv: %s\n", buff);

    close(sock_fd);
        
    return 0;
}
