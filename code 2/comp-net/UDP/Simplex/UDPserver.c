#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8000);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    char message[1024];
    struct sockaddr_in client_address;
    socklen_t client_address_length = sizeof(client_address);

    while (1) {
        recvfrom(server_socket, message, sizeof(message), 0,
                                         (struct sockaddr *)&client_address, &client_address_length);

        printf("Received from %s:%d: %s\n",
               inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), message);

        // printf("Client: ");
        // fgets(message, sizeof(message), stdin);
        // sendto(server_socket, message, strlen(message), 0,
        //        (struct sockaddr *)&client_address, sizeof(client_address));
    }

    close(server_socket);

    return 0;
}