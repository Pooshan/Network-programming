#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define PORT 7878
#define IP_ADDRESS "127.0.0.1"

char *order = "GET /index.html HTTP/1.1 \r\n\r\n" ;

/* Note: spaces are delimiters and VERY important */

#define SIZE 8000

void pexit(char * msg)
{
	perror(msg);
	exit(1);
}

int main()
{
static struct sockaddr_in serv_addr;
int i,sock_fd;
char buff[SIZE];

	if((sock_fd = socket(AF_INET, SOCK_STREAM,0)) <0)
		pexit("socket() failed");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	serv_addr.sin_port = htons(PORT);

	if(connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <0)
	pexit("connect() failed");


	printf("Sent Command %s: \n", order);
	write(sock_fd, order, strlen(order));


	while( (i=read(sock_fd,buff,SIZE)) > 0)
	write(1,buff,i);
}
