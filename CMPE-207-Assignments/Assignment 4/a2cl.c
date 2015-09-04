#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#define IP_ADDRESS "127.0.0.1"

#define MAXL 100


#define SIZE 100

int main(int argc, char *argv[])
{
int sfd;
  int numbytes;
  struct sockaddr_in server_addr;
  char buf[SIZE];
  char usage[MAXL];
char msg1[30]="Client1";
  int port=13;
int i;
char buffer[MAXL];
 

 

  if ((sfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    return 1;
  }

    server_addr.sin_family=AF_INET;
  server_addr.sin_port= htons(port);
  server_addr.sin_addr.s_addr=inet_addr(IP_ADDRESS);
  if(connect(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <0) 
perror("connect");
write(sfd, msg1, strlen(msg1));
while( (i=read(sfd,buffer,MAXL)) > 0)
	write(1,buffer,i);
   
}
