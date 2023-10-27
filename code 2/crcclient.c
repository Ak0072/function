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
	int sockfd, connfd, n;
	struct sockaddr_in servaddr, cli;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	printf("Socket created successfully\n");
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(8080);
	
	connect(sockfd, (SA*)&servaddr, sizeof(servaddr));
	printf("Connected to the server\n");
	
	printf("\nEnter data to be transmitted: ");  
	scanf("%s",data);  
	printf("\nEnter the Generating polynomial: ");  
	scanf("%s",gen_poly);
	data_length=strlen(data);
	for(i=data_length;i<data_length+N-1;i++)  
		data[i]='0'; 

	printf("\n----------------------------------------");    
	printf("\n Data padded with n-1 zeros : %s",data);  
	printf("\n----------------------------------------");  
	crc();
	for(i=data_length;i<data_length+N-1;i++)  
        	data[i]=check_value[i-data_length];    
	printf("\n----------------------------------------");  
	printf("\n Final data to be sent : %s",data);  
	printf("\n----------------------------------------\n");
	send(sockfd, data, 80, 0);
	send(sockfd, gen_poly, 10 ,0);
	send(sockfd, check_value, 80, 0);
	close(sockfd);
}
  
