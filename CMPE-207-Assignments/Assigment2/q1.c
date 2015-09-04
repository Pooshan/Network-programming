#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/time.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdarg.h>
//#include<stdio.h>
#include<stdlib.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
#include<errno.h>
#include<time.h>

//#include<stdlib.h>
//#include<string.h>
#include<netdb.h>



int          errexit(const char* format,...);
int          TCPdaytimed(int fd);
int          passiveTCP(const char *service, int qlen);

# define QLEN  32

int
main (int argc,char *argv[])
{
struct sockaddr_in fsin;   /*the from address of a client */
char *service = "daytime"; //service name or port number
int msock,ssock;           /*master and slave socket*/
unsigned int alen;         /*from address length */


switch (argc){

case   1:
         break;

case   2:
         service = argv[1];
         break;

default:
         errexit("usage: TCPdaytimed [port] \n");

}

msock = passiveTCP(service, QLEN);

while(1) {
alen = sizeof(fsin);
ssock = accept(msock, (struct sockaddr *)&fsin, &alen);
if(ssock< 0)
errexit("accept failed: %s \n",strerror(errno));
switch(fork())
{
case 0: // child
//(void) TCPdaytimed(ssock);
TCPdaytimed(ssock);
(void) close(ssock);
exit(0);
default:
close(ssock);
break;
case -1:
errexit("fork: %s\n", strerror(errno));
}


}
}

TCPdaytimed(int fd)
{
char  pts[70];               /*pointer to time string*/
time_t now;
//char *ctime();              /*current time */
struct tm * tm;
(void) time(&now);
tm=localtime(&now);

strftime(pts,70, "\n%m/%d/%Y %H:%M:%S %a\n",tm);
//(void) time(&now);
//pts = ctime(&now);
(void) write(fd,pts,strlen(pts));

}
/* passivesock.c - passivesock */

//extern int  errno;

int errexit(const char *format,...);

unsigned short portbase = 0; /* port base,for non-root servers */

int
passivesock(const char *service,const char*transport, int qlen)

{
	struct servent *pse;
	struct protoent *ppe;
	struct sockaddr_in sin;
	int s, type;

	memset(&sin,0,sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;

	//if( pse=getserverbyname(service, transport) )
	//sin.sin_port = htons(ntohs((unsigned short)pse->s_port+portbase));
	sin.sin_port = htons(8888);

	//else if((sin.sin_port=htons((unsigned short)atoi(service)))==0)
	//errexit("can't get \"%s\"service entry\n",service);
	if((ppe= getprotobyname(transport))==0)
	errexit("cant grt \"%s\"service entry\n",service);

	if(strcmp(transport,"udp")==0)
	type =SOCK_DGRAM;
	else
	type=SOCK_STREAM;
	s=socket(PF_INET,type,ppe->p_proto);
	if(s<0)
	errexit("cant create socket:%s \n",strerror(errno));
	if(bind(s,(struct sockaddr*)&sin,sizeof(sin))<0)
	errexit("cant bind to %s port: %s \n",service,strerror(errno));
	if(type ==SOCK_STREAM &&listen(s,qlen)<0)
	errexit("cant listen on %s port :%s \n",service,strerror(errno));
	return s;

		}

int errexit(const char *format,...)
{
   va_list args;

   va_start(args,format);
   vfprintf(stderr, format, args);
   va_end(args);
   exit(1);
}
int passiveTCP(const char *service, int qlen)
{
return passivesock(service,"tcp",qlen);
}



