#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../Structures/constants.h"
#include "../Structures/customer.h"
#include "../Structures/employee.h"

#ifndef COMMON
#define COMMON

bool exit_handler(int client_socket);
bool logout(int client_socket);

bool logout(int client_socket) {
    int write_bytes = write(client_socket, LOGOUT, strlen(LOGOUT));
    if (write_bytes == -1) {
        perror("Writing logout to client\n");
        return false;
    }
    return true;
}

bool exit_handler(int client_socket) {
    int write_bytes = write(client_socket, EXIT, strlen(EXIT));
    if (write_bytes == -1) {
        perror("Writing exit to client\n");
        return false;
    }
    return true;
}

#endif