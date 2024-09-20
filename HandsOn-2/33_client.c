/*
========================================================================================================
Name : 33
Author : Himanshu Rawat
Description : Write a program to communicate between two machines using socket.
Date: 20 Sept, 2024.
========================================================================================================
*/

//Client


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <unistd.h>

void main() {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("Error when creating socket");
	}
	printf("Socket created\n");

	struct sockaddr_in addr;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);

	int conn = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
	if (conn == -1) {
		perror("Error while establishing connection");
	}
	printf("Connection with server established\n");

	char buffer[100];
	read(sock, buffer, 100);
	printf("Data from server: %s\n", buffer);

	printf("Write message for server: ");
	scanf("%[^\n]", buffer);
	write(sock, buffer, sizeof(buffer));
	printf("Data sent to server\n");

	close(sock);
}

/*
========================================================================================================
Output:
./b
Socket created
Connection with server established
Data from server: hello
Write message for server: hello
Data sent to server

========================================================================================================
*/
