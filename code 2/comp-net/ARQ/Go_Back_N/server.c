#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

typedef struct {
    int sequence_number;
    char data[1024];
} Frame;

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8000);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    listen(server_socket, 5);

    int client_socket = accept(server_socket, NULL, NULL);

    Frame expected_frame;
    expected_frame.sequence_number = 0;

    char ackn[1024];
    int frame_count = 0;

    while (1) {
        Frame received_frame;
        recv(client_socket, &received_frame, sizeof(Frame), 0);

        if (received_frame.sequence_number == expected_frame.sequence_number) {
            printf("Received frame %d: %s\n", received_frame.sequence_number, received_frame.data);
            strcpy(ackn, "ACK");
            send(client_socket, ackn, sizeof(ackn), 0);
            expected_frame.sequence_number++;
        } else {
            printf("Received out-of-sequence frame: %d (expected: %d)\n", received_frame.sequence_number, expected_frame.sequence_number);
            strcpy(ackn, "N-ACK");
            send(client_socket, ackn, sizeof(ackn), 0);
        }

        frame_count++;
    }

    close(server_socket);
    close(client_socket);
    return 0;
}
