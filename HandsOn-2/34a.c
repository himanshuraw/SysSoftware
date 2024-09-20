/*
========================================================================================================
Name : 34.a
Author : Himanshu Rawat
Description : Write a program to create a concurrent server using fork.
Date: 20 Sept, 2024.
========================================================================================================
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <unistd.h>

int main() {
	int srvSock = socket(AF_INET, SOCK_STREAM, 0);
	if (srvSock < 0) {
		perror("Socket creation failed");
		return 1;
	}
	printf("Server socket successfully created.\n");

	struct sockaddr_in srvAddr = {0}, cliAddr = {0};
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvAddr.sin_port = htons(8080);

	if (bind(srvSock, (struct sockaddr *)&srvAddr, sizeof(srvAddr)) < 0) {
		perror("Binding failed");
		close(srvSock);
		return 1;
	}
	printf("Binding successful.\n");

	if (listen(srvSock, 3) < 0) {
		perror("Listening failed");
		close(srvSock);
		return 1;
	}
	printf("Waiting for connections...\n");

	while (1) {
		socklen_t cliAddrSize = sizeof(cliAddr);
		int cliSock = accept(srvSock, (struct sockaddr *)&cliAddr, &cliAddrSize);
		if (cliSock < 0) {
			perror("Connection acceptance failed");
			continue;
		}

		if (fork() == 0) {
			close(srvSock);

			char msgBuf[100];
			printf("Input message for client: ");
			fgets(msgBuf, sizeof(msgBuf), stdin);
			write(cliSock, msgBuf, sizeof(msgBuf));
			
			read(cliSock, msgBuf, sizeof(msgBuf));
			printf("Received from client: %s\n", msgBuf);
			
			close(cliSock);
			return 0;
		} else {
			close(cliSock);
		}
	}

	close(srvSock);
}

/*
========================================================================================================

Output:

Sever:
./a.out
Server socket successfully created.
Binding successful.
Waiting for connections...
Input message for client: first messageInput message for client:
second message
Received from client: thanks from first
Received from client: thanks from second
^C

Client 1:
./b
Socket created
Connection with server established
Data from server: first message

Write message for server: thanks from first
Data sent to server


Client 2:
./b
Socket created
Connection with server established
Data from server: second message

Write message for server: thanks from second
Data sent to server

========================================================================================================
*/
