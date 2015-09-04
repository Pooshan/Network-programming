#include<sys/types.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdarg.h>
#include<pthread.h>
#include<stdlib.h>
#include<errno.h>
#include<time.h>
#include<netdb.h>

extern int errno;

int daytime(char buf[]);
int errexit(const char *format, ...);
int passiveTCP(const char *service, int qlen);
int passiveUDP(const char *service);
void *connection_handlerudp(void *socket_desc);
void *connection_handler(void *socket_desc);

int x, y;
#define MAX(X,Y)	((x) > (y) ? (x):(y))

#define QLEN	32

#define	LINELEN	128
#define port  8888

int main(int argc, char*argv[]) {
char *service = "daytime";
	char buf[LINELEN + 1];
	struct sockaddr_in fsin;
	unsigned int alen;
	int tsock;
	int usock;
	int nfds;
	fd_set rfds;


	tsock = passiveTCP("tcp", QLEN);
	usock = passiveUDP("udp");
	nfds = MAX(tsock, usock) + 1;

	FD_ZERO(&rfds);

	while (1) {
		FD_SET(tsock, &rfds);
		(usock, &rfds);

		if (select(nfds, &rfds, (fd_set *) 0, (fd_set *) 0,
				(struct timeval *) 0) < 0)
			errexit("select error : %s\n", strerror(errno));

		if (FD_ISSET(tsock, &rfds)) {
			int ssock;
			pthread_t thread_id;
			alen = sizeof(fsin);
			ssock = accept(tsock, (struct sockaddr *) &fsin, &alen);
			if (ssock < 0)
				errexit("accept failed: %s\n", strerror(errno));

			if (pthread_create(&thread_id, NULL, connection_handler,
					(void*) &ssock) < 0) {
				perror("could not create thread");
				return 1;
			}

			daytime(buf);
			(void) write(ssock, buf, strlen(buf));
			(void) close(ssock);
		}

		if (FD_ISSET(usock, &rfds)) {
			alen = sizeof(fsin);
			pthread_t thread_id;
			if (recvfrom(usock, buf, sizeof(buf), 0, (struct sockaddr *) &fsin,
				&alen) < 0)
			errexit("recvfrom: %s\n", strerror(errno));

			if (pthread_create(&thread_id, NULL, connection_handlerudp,
					(void*) &usock) < 0) {
				perror("could not create thread");
				return 1;
			}
			daytime(buf);
			(void) sendto(usock, buf, strlen(buf), 0, (struct sockaddr*) &fsin,
					sizeof(fsin));
		}
}

void *connection_handlerudp(void *socket_desc) {

	char buf[LINELEN + 1];
	unsigned int alen;
	struct sockaddr_in fsin;
	int readsize;
	int sock = *(int*) socket_desc;
	alen = sizeof(fsin);
	while ((readsize = recvfrom(sock, buf, sizeof(buf), 0,
			(struct sockaddr *) &fsin, &alen)) > 0) {
		sendto(sock, buf, sizeof(buf), 0, (struct sockaddr *) &fsin,
				sizeof(fsin));
	}

	if (readsize == 0) {
		puts("Client disconnected");
		fflush(stdout);
	} else if (readsize == -1) {
		perror("recv failed");
	}
	errexit("recvfrom: %s\n", strerror(errno));


}
void *connection_handler(void *socket_desc) {
//Get the socket descriptor
	int sock = *(int*) socket_desc;
	int read_size;
	char *message, client_message[2000];

//Send some messages to the client
	message = "Greetings! I am your connection handler\n";
	write(sock, message, strlen(message));

	message = "Now type something and i shall repeat what you type \n";
	write(sock, message, strlen(message));

//Receive a message from client
	while ((read_size = recv(sock, client_message, 2000, 0)) > 0) {
	//	end of string marker
		client_message[read_size] = '\0';

		//Send the message back to client
		write(sock, client_message, strlen(client_message));

		//clear the message buffer
		memset(client_message, 0, 2000);
	}

	if (read_size == 0) {
		puts("Client disconnected");
		fflush(stdout);
	} else if (read_size == -1) {
		perror("recv failed");
	}

	return 0;
}

int daytime(char buf[]) {
	char *ctime();
	time_t now;

	(void) time(&now);
	sprintf(buf, "%s", ctime(&now));
	return(0);

}
int passiveTCP(const char *service, int qlen) {
	return passivesock(service, "tcp", qlen);
}

int passivesock(const char *service, const char*transport, int qlen)

{
	struct servent *pse;
	struct protoent *ppe;
	struct sockaddr_in sin;
	int s, type;

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;

	if( pse=getserverbyname(service, transport) ){
	sin.sin_port = htons(ntohs((unsigned short)pse->s_port+8888));
	sin.sin_port = htons(port);
	}
	else

	((sin.sin_port=htons((unsigned short)atoi(service)))==0);

	{errexit("can't get \"%s\"service entry\n",service); }
	if ((ppe = getprotobyname(transport)) == 0)
		errexit("cant grt \"%s\"service entry\n", service);

	if (strcmp(transport, "udp") == 0)
		type = SOCK_DGRAM;
	else
		type = SOCK_STREAM;
	s = socket(PF_INET, type, ppe->p_proto);
	if (s < 0)
		errexit("cant create socket:%s \n", strerror(errno));
	if (bind(s, (struct sockaddr*) &sin, sizeof(sin)) < 0)
		errexit("cant bind to %s port: %s \n", service, strerror(errno));
	if (type == SOCK_STREAM && listen(s, qlen) < 0)
		errexit("cant listen on %s port :%s \n", service, strerror(errno));
	return s;

}
int passiveUDP(const char *service) {
	return passivesock(service, "udp", 0);
}

int errexit(const char *format, ...) {
	va_list args;

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	exit(1);
}
}

