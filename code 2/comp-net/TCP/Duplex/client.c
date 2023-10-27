#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in client_address;
    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &client_address.sin_addr);

    connect(client_socket, (struct sockaddr *)&client_address, sizeof(client_address));

    char message[1024];
    while (1) {
        printf("Client: ");
        fgets(message, sizeof(message), stdin);
        send(client_socket, &message, sizeof(message), 0);

        recv(client_socket, &message, sizeof(message), 0);
        printf("Server ACK", message);
    }

    close(client_socket);

    return 0;
}