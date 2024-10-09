#include <unistd.h>

#include "../Structures/customer.h"

#ifndef EMPLOYEE
#define EMPLOYEE

struct Employee employee;

bool employee_handler(int client_socket);
bool login_employee(int client_socket);
int add_customer(int client_socket);

bool employee_handler(int client_socket) {
    char read_buffer[1000], write_buffer[1000];
    int read_bytes, write_bytes;
    if (!login_employee(client_socket)) {
        return false;
    }
    printf("working with employee\n");

    while (true) {
        write_bytes =
            write(client_socket, EMPLOYEE_MENU, strlen(EMPLOYEE_MENU));
        if (write_bytes == -1) {
            perror("Writing employee menu\n");
            return false;
        }

        read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
        if (read_bytes == -1) {
            perror("Reading choice from client");
        }

        int choice = atoi(read_buffer);
        switch (choice) {
            case 1:
                add_customer(client_socket);
                break;

            default:
                logout(client_socket);
                return true;
        }
    }
}

bool login_employee(int client_socket) {
    char read_buffer[1000], write_buffer[1000], buffer[1000];
    int read_bytes, write_bytes;
    int ID;

    memset(read_buffer, 0, sizeof(read_buffer));
    memset(write_buffer, 0, sizeof(write_buffer));

    strcpy(write_buffer, EMPLOYEE_LOGIN_PAGE);
    strcat(write_buffer, USERNAME);

    write_bytes = write(client_socket, write_buffer, strlen(write_buffer));
    if (write_bytes == -1) {
        perror("Write login page");
        return false;
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Read Username");
        return false;
    }

    if (read_bytes == 0) {
        perror("No input");
        return false;
    }

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, read_buffer);
    strtok(buffer, "-");
    ID = atoi(strtok(NULL, "-"));

    int file_fd = open(EMPLOYEE_FILE, O_RDONLY);
    if (file_fd == -1) {
        perror("Open employee file");
        return false;
    }

    off_t offset = lseek(file_fd, ID * sizeof(struct Employee), SEEK_SET);

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = ID * sizeof(struct Employee);
    lock.l_len = sizeof(struct Employee);
    lock.l_pid = getpid();

    int lock_status = fcntl(file_fd, F_SETLKW, &lock);
    if (lock_status == -1) {
        perror("Read lock\n");
        return false;
    }

    read_bytes = read(file_fd, &employee, sizeof(struct Employee));
    if (read_bytes == -1) {
        perror("Reading file\n");
    }
    lock.l_type = F_UNLCK;
    fcntl(file_fd, F_SETLK, &lock);

    if (strcmp(employee.username, read_buffer) != 0) {
        write_bytes =
            write(client_socket, INVALID_USERNAME, strlen(INVALID_USERNAME));
        return false;
    }

    close(file_fd);

    write_bytes = write(client_socket, PASSWORD, strlen(PASSWORD));
    if (write_bytes == -1) {
        perror("Writing password in employee\n");
        return false;
    }

    memset(read_buffer, 0, sizeof(read_buffer));
    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Read password in employee\n");
        return false;
    }

    char hashed_password[1000];
    strcpy(hashed_password, crypt(read_buffer, SALT));

    if (strcmp(hashed_password, employee.password) != 0) {
        write_bytes =
            write(client_socket, INVALID_PASSWORD, strlen(INVALID_PASSWORD));
        return false;
    }

    return true;
}

int add_customer(int client_socket) {
    char read_buffer[1000], write_buffer[1000];  //, buffer[1000]
    int read_bytes, write_bytes;

    struct Customer new_customer, prev_customer;

    // Name
    write_bytes = write(client_socket, ASK_NAME, strlen(ASK_NAME));
    if (write_bytes == -1) {
        perror("Writing name while adding");
        return -1;
    }

    memset(read_buffer, 0, sizeof(read_buffer));
    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Reading name while adding");
        return -1;
    }

    strcpy(new_customer.name, read_buffer);

    // Age
    write_bytes = write(client_socket, ASK_AGE, strlen(ASK_AGE));
    if (write_bytes == -1) {
        perror("Writing age while adding");
        return -1;
    }

    memset(read_buffer, 0, sizeof(read_buffer));
    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Reading name while adding");
        return -1;
    }

    int age = atoi(read_buffer);
    new_customer.age = age;

    // Gender
    write_bytes = write(client_socket, ASK_GENDER, strlen(ASK_GENDER));
    if (write_bytes == -1) {
        perror("Writing gender while adding");
        return -1;
    }

    memset(read_buffer, 0, sizeof(read_buffer));
    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Reading gender while adding");
        return -1;
    }

    char gender = read_buffer[0];

    if (gender == 'M' || gender == 'F' || gender == 'O') {
        new_customer.gender = gender;
    } else {
        write_bytes =
            write(client_socket, INVALID_ENTRY, strlen(INVALID_ENTRY));
        read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
        return -1;
    }

    // Active status
    new_customer.is_active = true;

    // Balance
    write_bytes = write(client_socket, ASK_BALANCE, strlen(ASK_BALANCE));
    if (write_bytes == -1) {
        perror("Writing age while adding");
        return -1;
    }

    memset(read_buffer, 0, sizeof(read_buffer));
    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Reading name while adding");
        return -1;
    }

    float balance = atof(read_buffer);
    new_customer.balance = balance;

    // Account number
    int customer_fd = open(CUSTOMER_FILE, O_RDWR);
    if (customer_fd == -1) {
        if (errno == ENOENT) {
            new_customer.account_number = 1;
        } else {
            perror("opening customer.txt to add employee");
            close(customer_fd);
            return -1;
        }
    } else {
        int offset = lseek(customer_fd, -sizeof(struct Customer), SEEK_END);
        if (offset == -1) {
            perror("Seek last customer");
            close(customer_fd);
            return -1;
        }
        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset;
        lock.l_len = sizeof(struct Customer);
        lock.l_pid = getpid();

        int lock_status = fcntl(customer_fd, F_SETLKW, &lock);
        if (lock_status == -1) {
            perror("Read lock on customer file\n");
            close(customer_fd);
            return -1;
        }
        read_bytes = read(customer_fd, &prev_customer, sizeof(struct Customer));
        if (read_bytes == -1) {
            perror("Read from customer file\n");
            close(customer_fd);
            return -1;
        }
        lock.l_type = F_UNLCK;
        fcntl(customer_fd, F_SETLK, &lock);
        close(customer_fd);
        new_customer.account_number = prev_customer.account_number + 1;
    }

    // Username
    memset(write_buffer, 0, sizeof(write_buffer));
    strcpy(new_customer.username, new_customer.name);
    strcat(new_customer.username, "-");
    sprintf(write_buffer, "%d", new_customer.account_number);
    strcat(new_customer.username, write_buffer);

    // Password
    char hashed_password[1000];
    strcpy(hashed_password, crypt(new_customer.name, SALT));
    strcpy(new_customer.password, hashed_password);

    customer_fd = open(CUSTOMER_FILE, O_CREAT | O_WRONLY | O_APPEND, 0777);
    if (customer_fd == -1) {
        perror("Opening customeer file");
        return -1;
    }

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_END;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    int lock_status = fcntl(customer_fd, F_SETLKW, &lock);
    if (lock_status == -1) {
        perror("Write lock on customer file");
        close(customer_fd);
        return -1;
    }

    write_bytes = write(customer_fd, &new_customer, sizeof(new_customer));
    if (write_bytes == -1) {
        perror("Adding new customer");
        return -1;
    }

    lock.l_type = F_UNLCK;
    lock_status = fcntl(customer_fd, F_SETLK, &lock);
    if (lock_status == -1) {
        perror("unlock write-lock from customer file");
        close(customer_fd);
        return -1;
    }

    close(customer_fd);

    memset(write_buffer, 0, sizeof(write_buffer));
    sprintf(write_buffer, "%s%s-%d\n%s%s", AUTOGEN_USERNAME, new_customer.name,
            new_customer.account_number, AUTOGEN_PASSWORD, new_customer.name);

    write_bytes = write(client_socket, write_buffer, sizeof(write_buffer));
    if (write_bytes == -1) {
        perror("Writing to client about new customer");
        return -1;
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));

    return new_customer.account_number;
}

#endif