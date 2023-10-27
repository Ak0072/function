#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8000);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);

    char message[1024];
    socklen_t server_address_length = sizeof(server_address);

    while (1) {
        printf("Client: ");
        fgets(message, sizeof(message), stdin);
        sendto(client_socket, message, strlen(message), 0,
               (struct sockaddr *)&server_address, sizeof(server_address));

        recvfrom(client_socket, message, sizeof(message), 0,
                                         (struct sockaddr *)&server_address, &server_address_length);

        printf("Received from %s:%d: %s\n",
               inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port), message);
    }
    close(client_socket);
    return 0;
}