#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>
#include <poll.h>


#define SERV_PORT 8888
#define BUF_SIZE 256

int seq = 1;

typedef struct frame{
	int frame_no;
	int seqno;
	char data[BUF_SIZE];
	}frame;

void verify(frame* f1){
	printf("Message is %s\n", f1->data);
	int seq_to_receive = seq;
	if(f1->seqno != seq_to_receive){
		printf("Duplicate frame, so discarded\n");
	}
	else{
		printf("Frame Accepted\n");
		seq = (seq == 0)?1:0;
	}
}

int main()
{
	int sockfd, client_socket;
	struct sockaddr_in server_addr, client_addr;


	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){ // creation
		printf("Socket Creation Error\n");
		exit(-1);
		}

	memset(&(server_addr), 0, sizeof(server_addr));
	memset(&(client_addr), 0, sizeof(client_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(SERV_PORT);

	if(bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		printf("Binding error\n");
		close(sockfd);
		exit(-1);
		}
	if(listen(sockfd, 2) < 0){
		printf("Listening error\n");
		close(sockfd);
		exit(-1);
		}
	printf("Server listening on port %d...\n", SERV_PORT);

	socklen_t client_addr_size = sizeof(client_addr);
	client_socket = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_size);
	if(client_socket < 0){
		printf("Con error\n");
		exit(-1);
		}

	printf("Connected to server\n");
	
	struct pollfd fds[1];
	fds[0].fd = 0;
	fds[0].events = POLLIN;
	int timeout = 10000;

	while(1){
		frame f1;
		
		char buffer[sizeof(frame)] = {0};
		int bytes_received = 0;
		
		memset(buffer, 0, sizeof(buffer));
		bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
		
		if (bytes_received < 0) {
            perror("Receiving failed");
            exit(EXIT_FAILURE);
        }
		if(bytes_received == 0){
			printf("Client Disconnected\n");
			break;
		}
		
		
		memcpy(&f1, buffer, sizeof(frame));
		verify((frame*)&f1);
		
		char ack_message[] = "Acknowledged";
		
		printf("Should we send the acknowledgement? Press 1 within 10 seconds or 0 if u want to exit\n");
		
		int result = poll(fds, 1, timeout);
		if (result <= 0){
			printf("No response/ Polling Failed\n");
			continue;
		}
		int n; //user choice
		scanf("%d", &n);
		if(n == 1){
			if(send(client_socket, ack_message, sizeof(ack_message), 0) < 0 ){
				printf("Sending Error\n");
			}
		}
		else if (n == 0){
			printf("Exitting Program\n");
			break;
		}
	}
	
	close(client_socket);
	close(sockfd);
	printf("Program exitted Successfully\n");
	
	return 0;
}



		
		
			
		

