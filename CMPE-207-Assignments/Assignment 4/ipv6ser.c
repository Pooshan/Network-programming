#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>

#include <arpa/inet.h>
#define MAX 1000

void error(char *msg) {
    perror(msg);
    exit(1);
}
 

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
struct sockaddr_in c_addr;
  time_t t;
  struct tm *tm;
char usage[MAX];
  char msg8[MAX],msg1[MAX],msg2[MAX],msg3[MAX],msg4[MAX],msg5[MAX],msg6[MAX],msg7[MAX];

    struct sockaddr_in6 serv_addr, cli_addr;
    int n;
    char client_addr_ipv6[100];



    printf("\nIPv6 TCP Server Started...\n");
  
    sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));
 
portno=13;    
serv_addr.sin6_flowinfo = 0;
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_addr = in6addr_any;
    serv_addr.sin6_port = htons(portno);

    
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

 
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    
    
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");

    inet_ntop(AF_INET6, &(cli_addr.sin6_addr),client_addr_ipv6, 100);
    printf("Incoming connection from client having IPv6 address: %s\n",client_addr_ipv6);

    memset(msg8,0, 256);
    
    //Sockets Layer Call: recv()
//    n = recv(newsockfd, buffer, 255, 0);
  //  if (n < 0)
    //    error("ERROR reading from socket");

    //printf("Message from client: %s\n", buffer);
// now me

      read( newsockfd , buffer, 1024);
if ((t = time(NULL)) < 0)
        {
          perror("daytimed-tcp time");
          return 5;
        }
	
   
	tm = localtime(&t);
	if((tm->tm_wday==0) || (tm->tm_wday==6))
	{
	strcpy(msg7,"WeekEnd");
	}
	else
	{
	strcpy(msg7,"Weekday");
	}
      sprintf(msg1,"%.2i/",tm->tm_mon + 1);
	sprintf(msg2,"%.2i/",tm->tm_mday);
	sprintf(msg3,"%.4i",tm->tm_year + 1900);
	sprintf(msg4,"%.2i:",tm->tm_min);
	sprintf(msg5,"%.2i:",tm->tm_sec);
	sprintf(msg6,"%.2i",tm->tm_hour);

	strcat(msg8,msg1);
	strcat(msg8,msg2);
	strcat(msg8,msg3);
	strcat(msg8," ");
	strcat(msg8,msg4);
	strcat(msg8,msg5);
	strcat(msg8,msg6);
	strcat(msg8," ");
	strcat(msg8,msg7);
	puts(msg8);
      if ((send(newsockfd, msg8, strlen(msg8), 0)) < 0)
        {
          perror("daytimed-tcp send");
          return 6;
        }
   if (close(newsockfd) < 0)
        {
          perror("daytimed-tcp close");
          return 7;
        }

     

    
    
    return 0;
}
