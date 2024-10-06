#include <netinet/ip.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void connection_handler(int client_socket) {
    short choice;
    int read_bytes = read(client_socket, &choice, sizeof(choice));
    if (read_bytes < 0) {
        perror("Read\n");
        return;
    }
    choice = ntohs(choice);
    printf("Here: %hd\n", choice);
    // switch (choice) {
    //     case 1:
    //         customer_handler();
    //         break;
    //     case 2:
    //         bankEmployee();
    //         break;
    //     case 3:
    //         manager();
    //         break;
    //     case 4:
    //         admin();
    //         break;

    //     default:
    //         base();
    //         break;
    // }
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Server Socket\n");
        return 1;
    }

    struct sockaddr_in server_address, client_address;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8081);

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
        printf("Connection accepted\n");
        if (fork() == 0) {
            close(server_socket);
            connection_handler(client_socket);
            close(client_socket);
            _exit(0);
        }

        close(client_socket);
    }

    close(server_socket);
}