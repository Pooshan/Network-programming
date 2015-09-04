#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include <time.h>
#include<sys/time.h>

#include <stdlib.h>
#include <stdio.h>

int main(int argc , char *argv[])
{

    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
long lstart,lend,diff;
    struct timeval start, end;
    //Create socket
    sock = socket(AF_INET , SOCK_DGRAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 9999 );

    //Connect to remote server
  //  if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    //{
      //  perror("connect failed. Error");
       // return 1;
    //}

    puts("Connected\n");

    //keep communicating with server
    while(1)
    {
        printf("Enter message : ");
        scanf("%s" , message);

        //Send some data
        if( sendto(sock , message , strlen(message) , 0, (struct sockaddr *)&server , sizeof(server)) < 0)
        {
        	 puts("Send failed");
        	            return 1;
        }
        else
        {
        	//printf("testing1");
        	 gettimeofday(&start, NULL);
        	 lstart=((start.tv_sec * 1000000 + start.tv_usec)
        	  		  );
        }



        //Receive a reply from the server
        if( recvfrom(sock , server_reply , 2000 , 0,NULL,NULL) < 0)
        {
            puts("recv failed");
            break;
        }
        else
        {
        	//printf("testing2");
        	 gettimeofday(&end, NULL);
        	 //printf("testing3");
        	 lend=((end.tv_sec * 1000000 + end.tv_usec)
        	         	  		  );
        }
        diff=lend-lstart;
        printf(" time elapsed server reply : %ld\n", diff);
       // printf("%ld\n", lend);

        puts("Server reply :");
        puts(server_reply);
    }

    close(sock);
    return 0;
}

