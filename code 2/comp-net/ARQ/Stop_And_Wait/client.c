#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in client_address;
    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(8000);
    inet_pton(AF_INET, "127.0.0.1", &client_address.sin_addr);

    connect(client_socket, (struct sockaddr *)&client_address, sizeof(client_address));

    char message[1024];

    while (1) {
        printf("Client: ");
        fgets(message, sizeof(message), stdin);
        send(client_socket, message, strlen(message), 0);

        int ret;
        fd_set readfds;
        struct timeval timeout;

        FD_ZERO(&readfds);
        FD_SET(client_socket, &readfds);

        timeout.tv_sec = 5;  // Set the timeout to 5 seconds
        timeout.tv_usec = 0;

        int got_ack = 0;

        while (got_ack <= 0){

            ret = select(client_socket + 1, &readfds, NULL, NULL, &timeout);
            
            if (ret == 0) {
                printf("Timeout: No ACK received within 5 seconds, re-sending.\n");
                send(client_socket, message, strlen(message), 0);
            }

            got_ack = recv(client_socket, message, sizeof(message), 0);
            if (got_ack > 0) {
                printf("Server ACK: %s\n", message);
                break;
            } else{
                continue;
            }

        }
    }

    close(client_socket);

    return 0;
}