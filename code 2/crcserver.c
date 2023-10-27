#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define SA struct sockaddr
#define N strlen(gen_poly)

char data[80];   
char check_value[80];   
char gen_poly[10];  
int data_length,i,j; 

void XOR()
{   
	for(j = 1;j < N; j++)  
		check_value[j] = (( check_value[j] == gen_poly[j])?'0':'1');      
}  
  
void crc()
{ 
	for(i=0;i<N;i++)  
		check_value[i]=data[i];  
	do
	{   
		if(check_value[0]=='1')  
			XOR();  
		for(j=0;j<N-1;j++)  
			check_value[j]=check_value[j+1];   
		check_value[j]=data[i++];  
	}while(i<=data_length+N-1);   
}  

int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("Socket successfully created..\n");
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(8080);

	bind(sockfd, (SA*)&servaddr, sizeof(servaddr));
	printf("Socket successfully binded..\n");

	listen(sockfd, 5);
	printf("Server listening..\n");
	len = sizeof(cli);

	connfd = accept(sockfd, (SA*)&cli, &len);
	printf("Server accept the client...\n");
	recv(connfd, data, 80,0);
	recv(connfd, gen_poly, 10,0);
	recv(connfd, check_value, 80,0);
	data_length = strlen(data);
	/*data[strlen(data)-1] = '1';*/
	printf("\nData received: %s", data);
	crc();    
	for(i=0;(i<N-1) && (check_value[i]!='1');i++);  
	if(i<N-1)  
		printf("\nError detected\n\n");  
	else  
		printf("\nNo error detected\n\n");
	close(sockfd);
}

