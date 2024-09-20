/*
========================================================================================================
Name : 34b
Author : Himanshu Rawat
Description : Write a program to create a concurrent server using pthread_create.
Date: 20 Sept, 2024.
========================================================================================================
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void communicate(int *connectionfd) {
    char buf[100];
    write(*connectionfd, "Hello from server\n", 19);
    read(*connectionfd, buf, 100);
    printf("Received from client: %s\n", buf);
    close(*connectionfd);
}

int main() {
    int socktd = socket(AF_INET, SOCK_STREAM, 0);
    if (socktd == -1) {
        perror("Socket creation failed");
        return 1;
    }
    printf("Server socket created successfully.\n");

    struct sockaddr_in server, client;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    if (bind(socktd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("Binding to socket failed");
        close(socktd);
        return 1;
    }
    printf("Successfully bound to server socket.\n");

    if (listen(socktd, 2) == -1) {
        perror("Listening failed");
        close(socktd);
        return 1;
    }
    printf("Waiting for connections...\n");

    while (1) {
        int client_size = sizeof(client);
        int connectionfd = accept(socktd, (struct sockaddr *)&client, &client_size);
        if (connectionfd == -1) {
            perror("Connection acceptance failed");
            continue;
        } else {
            pthread_t threadID;
            if (pthread_create(&threadID, NULL, (void *)communicate, &connectionfd)) {
                perror("Thread creation failed");
            }
            pthread_detach(threadID); // Detach the thread to avoid memory leaks
        }
    }

    close(socktd);
    return 0;
}

/*
========================================================================================================
Output:

./a.out
Server socket created successfully.
Successfully bound to server socket.
Waiting for connections...

========================================================================================================
*/
