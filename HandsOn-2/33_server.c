/*
========================================================================================================
Name : 33
Author : Himanshu Rawat
Description : Write a program to communicate between two machines using socket.
Date: 12 Sept, 2024.
========================================================================================================
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <unistd.h>

void main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("Error when creating socket");
	}
	printf("Server socket created\n");

	struct sockaddr_in srv_addr, cli_addr;
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(8080);

	int bindStatus = bind(sockfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
	if (bindStatus == -1) {
		perror("Error while binding name to socket");
		_exit(0);
	}
	printf("Binding to server socket was successful\n");

	int listenStatus = listen(sockfd, 2);
	if (listenStatus == -1) {
		perror("Error while trying to listen to connections");
		_exit(0);
	}
	printf("Listening for connections\n");

	int cli_size = (int)sizeof(cli_addr);
	int connfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_size);
	if (connfd == -1) {
		perror("Error while accepting connection");
		_exit(0);
	} else {
		char buffer[100];
		printf("Write message from server to client: ");
		scanf("%[^\n]", buffer);
		write(connfd, buffer, sizeof(buffer));
		read(connfd, buffer, 100);
		printf("Data from client: %s\n", buffer);
	}

	close(sockfd);
}
/*
========================================================================================================
Output:
./a
Server socket created
Binding to server socket was successful
Listening for connections
Write message from server to client: hello
Data from client: hello

========================================================================================================
*/
