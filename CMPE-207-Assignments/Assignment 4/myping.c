
#include <stdio.h>
#include <stdlib.h>
#include <linux/ip.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <linux/icmp.h>
#include <string.h>

#define IDN 6751 
 
char Addr_Destination[20];
char Addr_Source[20];

char* get_ip();
char* to_ip(char*); 
unsigned short check_sum(unsigned short *, int);
void args_parse(char**, char*, char* );
void usage1();

 
int main(int argc, char* argv[])
{
    struct iphdr* ip;
int sock_fd;
    int opt_val;
    int addr_len;
    int siz;

    struct iphdr* ip_reply;
    struct icmphdr* icmp;
    struct sockaddr_in connection;
    char* pkt;
    char* buff;
        int i;
    int cnt=6570;
    
     
    if (getuid() != 0)
    {
    fprintf(stderr, "%s: root privelidges needed\n", *(argv + 0));
    exit(EXIT_FAILURE);
    }
 
    args_parse(argv, Addr_Destination, Addr_Source);
    strncpy(Addr_Destination, to_ip(Addr_Destination), 20);
    strncpy(Addr_Source, to_ip(Addr_Source), 20);
    printf("My Source address: %s\n", Addr_Source);
    printf("Destination address: %s\n", Addr_Destination);
     
    
    for(i=0;i<4;i++) //
  {
    cnt++;
    pkt = malloc(sizeof(struct iphdr) + sizeof(struct icmphdr));
    buff = malloc(sizeof(struct iphdr) + sizeof(struct icmphdr));
   
     
    ip = (struct iphdr*) pkt;
    icmp = (struct icmphdr*) (pkt + sizeof(struct iphdr));
     
    ip->ihl = 5;
    ip->version = 4;
    ip->tos= 0;
    ip->tot_len= sizeof(struct iphdr) + sizeof(struct icmphdr);
  
    ip->id= htons(cnt);
  
    ip->frag_off  = 0;
    ip->ttl  = 30;
    ip->protocol= IPPROTO_ICMP;
    ip->saddr= inet_addr(Addr_Source);
    ip->daddr= inet_addr(Addr_Destination);
    ip->check= check_sum((unsigned short *)ip, sizeof(struct iphdr));
    
    
    if ((sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
    {
    perror("socket");
    exit(EXIT_FAILURE);
    }
     
    
    setsockopt(sock_fd, IPPROTO_IP, IP_HDRINCL, &opt_val, sizeof(int));
     
   
    icmp->type = ICMP_ECHO;
    icmp->code= 0;
    icmp->un.echo.id  = random();
    icmp->un.echo.sequence = 0;
    icmp-> checksum = check_sum((unsigned short *)icmp, sizeof(struct icmphdr));
     
     
    connection.sin_family = AF_INET;
    connection.sin_addr.s_addr = inet_addr(Addr_Destination);
   


    sendto(sock_fd, pkt, ip->tot_len, 0, (struct sockaddr *)&connection, sizeof(struct sockaddr));
  
    printf("we Sent %d byte pkt to %s\n", ip->tot_len, Addr_Destination);
    printf("ID= %d\n", ntohs(ip->id));
   
    printf("TTL= %d\n", ip->ttl);
  
    addr_len = sizeof(connection);
    if (( siz = recvfrom(sock_fd, buff, sizeof(struct iphdr) + sizeof(struct icmphdr), 0, (struct sockaddr *)&connection, &addr_len)) == -1)
    {
    perror("recv");
    }
    else
    {
    printf("Received %d byte reply from %s:\n", siz , Addr_Destination);
        ip_reply = (struct iphdr*) buff;
    printf("ID: %d\n", ntohs(ip_reply->id));
    printf("TTL: %d\n", ip_reply->ttl);
    }}

    free(pkt);
    free(buff);
    close(sock_fd);
    return 0;
}

 
void usage1()
{
    fprintf(stderr, "\nusage1: pinger [destination] <-s [source]>\n");
    fprintf(stderr, "Destination must be provided\n");
    fprintf(stderr, "Source is optional\n\n");
}
 
char* get_ip()
{
struct hostent* h;
    char buff[256];
    
     
    gethostname(buff, 256);
    h = gethostbyname(buff);
     
    return inet_ntoa(*(struct in_addr *)h->h_addr);
     
}
 

char* to_ip(char* address)
{
    struct hostent* h;
    h = gethostbyname(address);
    return inet_ntoa(*(struct in_addr *)h->h_addr);
}
 
void args_parse(char** argv, char* destination, char* source)
{
    int i;
    if(!(*(argv + 1)))
    {
    
    usage1();
    exit(EXIT_FAILURE);
    }
    if (*(argv + 1) && (!(*(argv + 2))))
    {
   
    strncpy(destination, *(argv + 1), 15);
    strncpy(source, get_ip(), 15);
    return;
    }
    else if ((*(argv + 1) && (*(argv + 2))))
    {
    
    strncpy(destination, *(argv + 1), 15);
    i = 2;
    while(*(argv + i + 1))
    {
        if (strncmp(*(argv + i), "-s", 2) == 0)
        {
        strncpy(source, *(argv + i + 1), 15);
        break;
        }
        i++;
    }
    }
 
}
unsigned short check_sum(unsigned short *addr, int len)
{
    register int sum = 0;
    u_short ans = 0;
    register u_short *w = addr;
    register int nleft = len;
   
    while (nleft > 1)
    {
      sum += *w++;
      nleft -= 2;
    }
    
    if (nleft == 1)
    {
      *(u_char *) (&ans) = *(u_char *) w;
      sum += ans;
    }
    
    sum = (sum >> 16) + (sum & 0xffff);      
    sum += (sum >> 16);              
    ans = ~sum;             
    return (ans);
}
