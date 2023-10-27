#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8000);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    listen(server_socket, 5);

    int client_socket = accept(server_socket, NULL, NULL);

    char message[1024];
    char ackn[1024];
    int frame_count = 0;

    while (1) {

        printf("RDY NXT FRM %d\n", frame_count);

        if (frame_count == 3){
        	recv(client_socket, message, sizeof(message), 0);
            printf("CLIENT SENT ERRANEOUS: %s\nN-ACK FOR THIS!\n", message);
	    } else{
	    	recv(client_socket, message, sizeof(message), 0);
	        printf("CLIENT SENT: %s\n", message);

	        strcpy(ackn, "ACK");
	        send(client_socket, ackn, sizeof(ackn), 0);
	    }
        frame_count++;
    }
    close(server_socket);
    close(client_socket);
    return 0;
}