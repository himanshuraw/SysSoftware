#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../Structures/constants.h"
#include "../Structures/employee.h"

#ifndef ADMINISTRATOR
#define ADMINISTRATOR

bool administrator_handler(int client_socket);
bool login_administrator(int client_socket);
int add_employee(int client_socket);

bool administrator_handler(int client_socket) {
    char read_buffer[1000], write_buffer[1000];  //, buffer[1000]
    int read_bytes, write_bytes;
    if (!login_administrator(client_socket)) {
        return false;
    }
    printf("Working with administrator\n");

    while (true) {
        write_bytes = write(client_socket, ADMINISTRATOR_MENU,
                            strlen(ADMINISTRATOR_MENU));
        if (write_bytes == -1) {
            perror("Writing admin menu\n");
            return false;
        }

        read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
        if (read_bytes == -1) {
            perror("Reading choice from client");
        }

        int choice = atoi(read_buffer);
        switch (choice) {
            case 1:
                add_employee(client_socket);
                break;

            default:
                break;
        }
    }
}

bool login_administrator(int client_socket) {
    char read_buffer[1000], write_buffer[1000];  //, buffer[1000]
    int read_bytes, write_bytes;

    memset(read_buffer, 0, sizeof(read_buffer));
    memset(write_buffer, 0, sizeof(write_buffer));

    strcpy(write_buffer, ADMINISTRATOR_LOGIN_PAGE);
    strcat(write_buffer, USERNAME);

    write_bytes = write(client_socket, write_buffer, strlen(write_buffer));
    if (write_bytes == -1) {
        perror("Write login page\n");
        return false;
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Read Username\n");
        return false;
    }

    if (read_bytes == 0) {
        perror("No input\n");
        return false;
    }

    if (strcmp(read_buffer, ADMINISTRATOR_USERNAME) != 0) {
        write_bytes =
            write(client_socket, INVALID_USERNAME, strlen(INVALID_USERNAME));
        return false;
    }

    write_bytes = write(client_socket, PASSWORD, strlen(PASSWORD));
    if (write_bytes == -1) {
        perror("Writing password in administrator\n");
        return false;
    }

    memset(read_buffer, 0, sizeof(read_buffer));
    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Read password in administrator\n");
        return false;
    }

    char hashed_password[1000];
    strcpy(hashed_password, crypt(read_buffer, SALT));

    if (strcmp(hashed_password, ADMINISTRATOR_PASSWORD) != 0) {
        write_bytes =
            write(client_socket, INVALID_PASSWORD, strlen(INVALID_PASSWORD));
        return false;
    }
    return true;
}

int add_employee(int client_socket) {
    char read_buffer[1000], write_buffer[1000];  //, buffer[1000]
    int read_bytes, write_bytes;

    struct Employee new_employee, prev_employee;

    int employee_fd = open(EMPLOYEE_FILE, O_RDONLY);

    if (employee_fd == -1) {
        if (errno == ENOENT) {
            new_employee.id = 0;
        } else {
            perror("opening employee.txt to add employee");
            return false;
        }
    } else {
        int offset = lseek(employee_fd, -sizeof(struct Employee), SEEK_END);
        if (offset == -1) {
            perror("Seeking last employee");
            return false;
        }
        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset;
        lock.l_len = sizeof(struct Employee);
        lock.l_pid = getpid();

        int lock_status = fcntl(employee_fd, F_SETLKW, &lock);
        if (lock_status == -1) {
            perror("Read lock on employee file\n");
            return false;
        }

        read_bytes = read(employee_fd, &prev_employee, sizeof(struct Employee));
        if (read_bytes == -1) {
            perror("Read from employee file");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(employee_fd, F_SETLK, &lock);
        close(employee_fd);
        new_employee.id = prev_employee.id + 1;
    }

    // Name
    write_bytes = write(client_socket, ASK_NAME, strlen(ASK_NAME));
    if (write_bytes == -1) {
        perror("Writing name while adding");
        return false;
    }

    memset(read_buffer, 0, sizeof(read_buffer));
    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Reading name while adding");
        return false;
    }

    strcpy(new_employee.name, read_buffer);

    // Age
    write_bytes = write(client_socket, ASK_AGE, strlen(ASK_AGE));
    if (write_bytes == -1) {
        perror("Writing age while adding");
        return false;
    }

    memset(read_buffer, 0, sizeof(read_buffer));
    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Reading name while adding");
        return false;
    }

    int age = atoi(read_buffer);
    new_employee.age = age;

    // Gender
    write_bytes = write(client_socket, ASK_GENDER, strlen(ASK_GENDER));
    if (write_bytes == -1) {
        perror("Writing gender while adding");
        return false;
    }

    memset(read_buffer, 0, sizeof(read_buffer));
    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Reading gender while adding");
        return false;
    }

    char gender = read_buffer[0];

    if (gender == 'M' || gender == 'F' || gender == 'O') {
        new_employee.gender = gender;
    } else {
        write_bytes =
            write(client_socket, INVALID_ENTRY, strlen(INVALID_ENTRY));
        read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
        return false;
    }

    // Username
    memset(write_buffer, 0, sizeof(write_buffer));
    strcpy(new_employee.username, new_employee.name);
    strcat(new_employee.username, "-");
    sprintf(write_buffer, "%d", new_employee.id);
    strcat(new_employee.username, write_buffer);

    // Password
    char hashed_password[1000];
    strcpy(hashed_password, crypt(new_employee.name, SALT));
    strcpy(new_employee.password, hashed_password);

    employee_fd = open(EMPLOYEE_FILE, O_CREAT | O_WRONLY | O_APPEND, 0777);
    if (employee_fd == -1) {
        perror("Opening employee file");
        return false;
    }
    write_bytes = write(employee_fd, &new_employee, sizeof(new_employee));
    if (write_bytes == -1) {
        perror("Adding new employee");
        return false;
    }

    close(employee_fd);

    memset(write_buffer, 0, sizeof(write_buffer));
    sprintf(write_buffer, "%s%s-%d\n%s%s", AUTOGEN_USERNAME, new_employee.name,
            new_employee.id, AUTOGEN_PASSWORD, new_employee.name);

    write_bytes = write(client_socket, write_buffer, sizeof(write_buffer));
    if (write_bytes == -1) {
        perror("Writing to client about new employee");
        return false;
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));

    return new_employee.id;
}

#endif