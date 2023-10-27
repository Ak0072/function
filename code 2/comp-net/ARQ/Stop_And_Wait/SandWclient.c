#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>

#define BUF_SIZE 256
#define SERVER_IP "127.0.0.1" // Update to the actual server IP
#define SERVER_PORT 8888

typedef struct frame {
    int frame_no;
    int seqno;
    char data[BUF_SIZE];
} frame;

void sendFrame(int client_socket, frame* framenew) {
    char buffer[sizeof(frame)] = {0};
    memcpy(buffer, framenew, sizeof(frame));
    send(client_socket, buffer, sizeof(buffer), 0);
}

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char ack_buf[256];

    // Create client socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare server address structure
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address.sin_port = htons(SERVER_PORT);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    int frame_no = 1;
    
    struct pollfd fds[1];
    fds[0].fd = client_socket;
    fds[0].events = POLLIN;
    fds[0].revents = 0;
    int timeout = 10000;
    
    while (1) {
        frame framenew;
        framenew.frame_no = frame_no;
        framenew.seqno = (frame_no % 2 == 0) ? 0 : 1;

        printf("Enter the data for frame %d: ", frame_no);
        fgets(framenew.data, sizeof(framenew.data), stdin);
        framenew.data[strcspn(framenew.data, "\n")] = '\0'; // Remove the newline character

        if (strcmp(framenew.data, "exit") == 0) {
            printf("Exiting the program\n");
            close(client_socket);
            break;
        }

        sendFrame(client_socket, &framenew);
		while (1) {

			int activity = poll(fds, 1, timeout);

            if (activity <= 0) {
                printf("Timeout has occurred. Retransmitting the frame...\n");
                sendFrame(client_socket, &framenew);
            }
            else {
            	memset(ack_buf, 0, sizeof(ack_buf));
            	recv(client_socket, ack_buf, sizeof(ack_buf), 0);
            	printf("Transmission Successful. Message from Server: %s \n", ack_buf);
                break;
            }
            
        }

        frame_no++; // Increment frame number
    }

    return 0;
}

