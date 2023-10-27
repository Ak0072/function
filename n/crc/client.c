#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#define SERV_TCP_PORT 5035
int main(int argc,char * * argv)
{
       int sockfd;
       struct sockaddr_in serv_addr;
       struct hostent *server;
       char a[30],b[30],q[30],r[30],crc[10];
       sockfd=socket(AF_INET,SOCK_STREAM,0);
       serv_addr.sin_family=AF_INET;
       serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
       serv_addr.sin_port=htons(SERV_TCP_PORT);
       connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
       printf("\nEnter the dividend:");
       scanf("%s",a);
       printf("\nEnter the divisor:");
       scanf("%s",b);
       write(sockfd,a,30);
       write(sockfd,b,30);
       printf("\n");
       printf("\nServer result:");
       read(sockfd,q,30);
       read(sockfd,r,30);
       read(sockfd,crc,10);
       printf("\n\nQuotient=%s",q);
       printf("\n\nRemainder=%s",r);
       printf("\n\nCRC values=%s\n",crc);
       close(sockfd);
       return 0;
}


