#include <netinet/ip.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "Structures/constants.h"

void connection_handler(int socket);

int main() {
    /*`````````````````` Socket creation ```````````````````````*/
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Client socket");
        return 1;
    }

    struct sockaddr_in address;
    // host's byte order -> network byte order
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_family = AF_INET;
    address.sin_port = htons(3000);

    /*````````````````` Connenting to Server ```````````````````*/
    int connection = connect(sock, (struct sockaddr*)&address, sizeof(address));
    if (connection == -1) {
        perror("Connection");
        close(sock);
        return 1;
    }

    printf("Connection Established\n");

    connection_handler(sock);

    printf("Ending connection");

    close(sock);
}

void connection_handler(int socket) {
    char read_buffer[1000], write_buffer[1000], buffer[1000];
    int read_bytes = 1, write_bytes;

    while (read_bytes) {
        memset(read_buffer, 0, sizeof(read_buffer));
        memset(write_buffer, 0, sizeof(write_buffer));

        read_bytes = read(socket, read_buffer, sizeof(read_buffer));
        if (read_bytes == -1) {
            perror("Read from client socket\n");
            return;
        }
        if (read_bytes == 0) {
            printf("Didn't receive anything from server\n");
            return;
        }
        if (strchr(read_buffer, '$') != NULL) {
            char* match = strchr(read_buffer, '$');
            int len = match - read_buffer;
            memcpy(buffer, read_buffer, len - 1);
            buffer[len - 1] = '\0';
            printf("%s\n", buffer);
            break;
        }

        memset(write_buffer, 0, sizeof(write_buffer));
        system("clear");
        printf("%s\n", read_buffer);
        scanf("%[^\n]%*c", write_buffer);
        write_bytes = write(socket, write_buffer, sizeof(write_buffer));
        if (write_bytes == -1) {
            perror("Write to client socket\n");
            return;
        }
    }
}