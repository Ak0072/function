#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#define WINDOW_SIZE 4 // Adjust window size as needed

typedef struct {
    int sequence_number;
    char data[1024];
} Frame;

int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in client_address;
    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(8000);
    inet_pton(AF_INET, "127.0.0.1", &client_address.sin_addr);

    connect(client_socket, (struct sockaddr *)&client_address, sizeof(client_address));

    int sequence_number = 0;
    int next_sequence_number = 0;
    char message[1024];

    Frame window[WINDOW_SIZE];
    int window_base = 0;

    while (1) {
        usleep(100000);
        
        while (next_sequence_number < window_base + WINDOW_SIZE) {
            printf("Client: ");
            fgets(message, sizeof(message), stdin);
            
            Frame frame;
            frame.sequence_number = next_sequence_number;
            strcpy(frame.data, message);
            window[next_sequence_number % WINDOW_SIZE] = frame;
            
            send(client_socket, &frame, sizeof(Frame), 0);
            
            next_sequence_number++;
        }

        while (1) {
            Frame ack_frame;
            int ret = recv(client_socket, &ack_frame, sizeof(Frame), MSG_DONTWAIT);
            
            if (ret > 0 && ack_frame.sequence_number >= window_base) {
                window_base = ack_frame.sequence_number + 1;
                printf("Server ACK: %d\n", ack_frame.sequence_number);
            } else {
                break;
            }
        }
    }

    close(client_socket);

    return 0;
}
