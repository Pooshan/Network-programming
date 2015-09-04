#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>
#define MYPORT 7878
#define QLEN 10
#define SIZE 8000
#define IP_ADD "127.0.0.1"

void sigchld_handler(int s)
{
    while(wait(NULL) > 0);
}

void* httpRequest(void* data)
{
    int sd = *((int*)data);
        char buff[SIZE],order[1000],tmp[200],tmp1[200];
        int a=0,k=0,b=0,received=-1,i=0;
char Status[100]="HTTP/1.1";
        char ContentType[100]="Content-type: ";

        if ((received = recv(sd, buff, 100, 0)) < 0)
        {
                perror("Failed to receive initial bytes from client");
        }
        buff[received-1]='\0';
        printf("Request Is here :) :\n");

        while(buff[i]!= '\n')

        {
        order[a]=buff[i++];
      a++;


        }


      int klm=0;
//Get the "GET" word.
        while(order[b] != ' ')
       {
 tmp[b]= buff[klm++];

b++;
}
//puts(tmp);
        if(strcmp(tmp,"GET")==0)
        {
      //Extract the file name.
               printf("Got a GET request:\n");
	}
          char *filename1,*filename2,*filename3;
               filename2=strpbrk(buff,"/");
//puts(filename2);
               filename3=strtok(filename2," ");
//puts(filename3);
     filename1=strpbrk(filename3,"/");
//puts(filename1);


b=0;
klm=1;
while(filename1[b] != ' ')
       {
 tmp1[b]= filename1[klm++];

b++;
}
puts(tmp1);
     //Open the requested file.
    FILE *fp;
    int file_exist=1;
    fp=fopen(tmp1, "r");
    if (fp==NULL)
             file_exist=0;
    printf("File opened:\n");


    char *filetype;
    char *type;
        int s='.';
    filetype=strrchr(filename1,s);

            if((strcmp(filetype,".htm"))==0 || (strcmp(filetype,".html"))==0)
            type="text/html";
        else if((strcmp(filetype,".jpg"))==0)
            type="image/jpeg";
        else if(strcmp(filetype,".gif")==0)
            type="image/gif";



        char mainbody[100]="<html>";
        if(file_exist==1)
        {

strcat(Status," 200 OK");
strcat(Status,"\r\n");
strcat(ContentType,type);
strcat(ContentType,"\r\n");

puts(Status);
puts(ContentType);
        }
        else //if file not avail
        {

            strcat(Status," 404 Not Found");
            strcat(Status,"\r\n");
            strcat(ContentType,"NONE");
            strcat(ContentType,"\r\n");
            puts(Status);
            puts(ContentType);
        }


    if ((send(sd, Status,strlen(Status), 0) == -1) ||
                (send(sd, ContentType, strlen(ContentType),0) == -1) ||
            (send(sd,"\r\n", strlen("\r\n"), 0) == -1))
                        perror("Failed to send bytes to client");
        //send the entity body
        if (file_exist)
        {
            wchar_t read_char[1];
            while((read_char[0]=fgetc(fp))!=EOF)
            {
                if(send(sd,read_char,sizeof(char),0) == -1)
                    perror("Failed to send bytes to client");
            }
        }
        else
        {
            if (send(sd, mainbody, 100, 0) == -1)
                        perror("Failed to send bytes to client");
               }
        close(sd);
       pthread_exit(NULL);


}

int main (void)
{
    int sock_fd, my_fd1;

        pthread_t thread[2000];
    int id,i=0;
int sin_size;
    struct sigaction sa;
struct sockaddr_in addr;
    struct sockaddr_in addr2;
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }



    // bzero((char *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(MYPORT);
    addr.sin_addr.s_addr = inet_addr(IP_ADD);
    memset(&(addr.sin_zero), '\0', 8);

    if (bind(sock_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr))== -1) {
        perror("bind");
        exit(1);
    }


    if (listen(sock_fd, QLEN) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    while(1)
   {

        sin_size = sizeof(struct sockaddr_in);
        if ((my_fd1 = accept(sock_fd, (struct sockaddr *)&addr2,&sin_size)) == -1) {
            perror("accept");
            continue;
        }

 id = pthread_create(&thread[i++],NULL,httpRequest,(void*)&my_fd1);
   }
    return 0;
}
