#include <netinet/ip.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "Functions/administrator.h"
#include "Functions/common.h"
#include "Functions/customer.h"
#include "Functions/employee.h"
#include "Functions/manager.h"
#include "Structures/constants.h"

void connection_handler(int client_socket);

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Server Socket\n");
        return 1;
    }

    struct sockaddr_in server_address, client_address;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(3000);

    int bind_status = bind(server_socket, (struct sockaddr *)&server_address,
                           sizeof(server_address));
    if (bind_status < 0) {
        perror("Binding\n");
        close(server_socket);
        return 1;
    }

    int listen_status = listen(server_socket, 10);
    if (listen_status == -1) {
        perror("Listen\n");
        close(server_socket);
        return 1;
    }

    while (true) {
        socklen_t client_address_size = sizeof(client_address);
        int client_socket =
            accept(server_socket, (struct sockaddr *)&client_address,
                   &client_address_size);
        if (client_socket == -1) {
            perror("Connection acceptance\n");
            continue;
        }

        /*``````````` Forking Child for new process `````````````*/
        if (fork() == 0) {
            close(server_socket);
            connection_handler(client_socket);
            printf("Terminating connection\n");
            close(client_socket);
            _exit(0);
        }

        close(client_socket);
    }

    close(server_socket);
}

void connection_handler(int client_socket) {
    printf("Client is connected to the server\n");

    char read_buffer[1000], write_buffer[1000];
    int read_bytes, write_bytes;
    int choice;

    write_bytes = write(client_socket, INITIAL_PROMPT, strlen(INITIAL_PROMPT));
    if (write_bytes == -1) {
        perror("Sending initial prompt\n");
        return;
    }
    memset(read_buffer, 0, sizeof(read_buffer));

    read_bytes = read(client_socket, &read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Reading choice\n");
        return;
    }
    if (read_bytes == 0) {
        printf("No data from client\n");
    }

    choice = atoi(read_buffer);

    switch (choice) {
        case 1:
            customer_handler(client_socket);
            break;
        case 2:
            employee_handler(client_socket);
            break;
        case 3:
            manager_handler(client_socket);
            break;
        case 4:
            administrator_handler(client_socket);
            break;
        default:
            exit_handler(client_socket);
            break;
    }
}