#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define MAX_CLIENTS 10
#define BUFFER 1024

int clients[MAX_CLIENTS];
int client_count = 0;

void broadcast(char *message) {
    for(int i=0;i<client_count;i++) {
        send(clients[i], message, strlen(message), 0);
    }
}

int main() {

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);

    printf("Broker TCP listening on port %d\n", PORT);

    while(1) {

        new_socket = accept(server_fd,(struct sockaddr*)&address,(socklen_t*)&addrlen);

        clients[client_count++] = new_socket;

        if(fork()==0) {

            while(1) {
                int valread = read(new_socket, buffer, BUFFER);
                if(valread <= 0) break;

                buffer[valread] = '\0';
                printf("Message: %s\n", buffer);

                broadcast(buffer);
            }

            close(new_socket);
            exit(0);
        }
    }
}