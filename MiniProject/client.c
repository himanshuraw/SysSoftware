#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

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
    address.sin_port = htons(8081);

    /*````````````````` Connenting to Server ```````````````````*/
    int connection =
        connect(sock, (struct sockaddr *)&address, sizeof(address));
    if (connection == -1) {
        perror("Connection");
        return 1;
    }

    printf("Connection Established %d\n", connection);
    printf("Log in as:\n");
    printf("1. Customer\n");
    printf("2. Bank Employee\n");
    printf("3. Manager\n");
    printf("4. Admin\n");

    short choice;
    scanf("%hd", &choice);
    choice = htons(choice);
    int write_byte = write(sock, &choice, sizeof(choice));
    printf("%d, %hd", write_byte, choice);

    close(sock);
}
