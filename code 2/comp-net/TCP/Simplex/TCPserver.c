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

    while (1) {
        recv(client_socket, &message, sizeof(message), 0);
        printf("Client: %s\n", message);

        // printf("Server: ");
        // fgets(message, sizeof(message), stdin);
        // send(client_socket, &message, sizeof(message), 0);
    }

    close(server_socket);
    close(client_socket);

    return 0;
}