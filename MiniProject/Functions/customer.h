#ifndef CUSTOMER
#define CUSTOMER

struct Customer customer;

bool customer_handler(int client_socket);
bool login_customer(int client_socket);
bool view_balance(int client_socket);
bool change_balance(int client_socket, int id, int amount);
bool deposit(int client_socket);
bool withdraw(int client_socket);
bool transfer_funds(int client_socket);

bool customer_handler(int client_socket) {
    char read_buffer[1000], write_buffer[1000];
    int read_bytes, write_bytes;
    if (!login_customer(client_socket)) {
        return false;
    }
    printf("working with customer\n");

    while (true) {
        write_bytes =
            write(client_socket, CUSTOMER_MENU, strlen(CUSTOMER_MENU));
        if (write_bytes == -1) {
            perror("Writing customer menu\n");
            return false;
        }

        read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
        if (read_bytes == -1) {
            perror("Reading choice from client\n");
        }

        int choice = atoi(read_buffer);
        switch (choice) {
            case 1:
                view_balance(client_socket);
                break;
            case 2:
                deposit(client_socket);
                break;
            case 3:
                withdraw(client_socket);
                break;
            case 4:
                transfer_funds(client_socket);
                break;
            default:
                logout(client_socket);
                return false;
        }
    }

    return true;
}

bool login_customer(int client_socket) {
    char read_buffer[1000], write_buffer[1000], buffer[1000];
    int read_bytes, write_bytes;
    int ID;

    memset(read_buffer, 0, sizeof(read_buffer));
    memset(write_buffer, 0, sizeof(write_buffer));

    strcpy(write_buffer, CUSTOMER_LOGIN_PAGE);
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

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, read_buffer);
    strtok(buffer, "-");
    ID = atoi(strtok(NULL, "-")) - 1;

    int file_fd = open(CUSTOMER_FILE, O_RDONLY);
    if (file_fd == -1) {
        perror("Open customer file\n");
        return false;
    }

    off_t offset = lseek(file_fd, ID * sizeof(struct Customer), SEEK_SET);
    if (offset == -1) {
        write_bytes =
            write(client_socket, INVALID_USERID, strlen(INVALID_USERID));
        return false;
    }

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = ID * sizeof(struct Customer);
    lock.l_len = sizeof(struct Customer);
    lock.l_pid = getpid();

    int lock_status = fcntl(file_fd, F_SETLKW, &lock);
    if (lock_status == -1) {
        perror("Read lock\n");
        close(file_fd);
        return false;
    }

    read_bytes = read(file_fd, &customer, sizeof(struct Customer));
    if (read_bytes == -1) {
        perror("Reading file\n");
    }
    lock.l_type = F_UNLCK;
    fcntl(file_fd, F_SETLK, &lock);

    if (strcmp(customer.username, read_buffer) != 0) {
        memset(write_buffer, 0, sizeof(write_buffer));
        write_bytes =
            write(client_socket, INVALID_USERNAME, strlen(INVALID_USERNAME));
        close(file_fd);
        return false;
    }

    close(file_fd);

    memset(write_buffer, 0, sizeof(write_buffer));
    write_bytes = write(client_socket, PASSWORD, strlen(PASSWORD));
    if (write_bytes == -1) {
        perror("Writing password in customer\n");
        return false;
    }

    memset(read_buffer, 0, sizeof(read_buffer));
    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Read password in customer\n");
        return false;
    }

    char hashed_password[1000];
    strcpy(hashed_password, crypt(read_buffer, SALT));

    if (strcmp(hashed_password, customer.password) != 0) {
        memset(write_buffer, 0, sizeof(write_buffer));
        write_bytes =
            write(client_socket, INVALID_PASSWORD, strlen(INVALID_PASSWORD));
        return false;
    }

    return true;
}

bool view_balance(int client_socket) {
    char read_buffer[1000], write_buffer[1000], buffer[100];
    int read_bytes, write_bytes;

    int id = customer.account_number - 1;

    int customer_fd = open(CUSTOMER_FILE, O_RDONLY);
    if (customer_fd == -1) {
        perror("Open customer file\n");
        return false;
    }

    off_t offset = lseek(customer_fd, id * sizeof(struct Customer), SEEK_SET);
    if (offset == -1) {
        write_bytes =
            write(client_socket, INVALID_USERID, strlen(INVALID_USERID));
        return false;
    }

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = id * sizeof(struct Customer);
    lock.l_len = sizeof(struct Customer);
    lock.l_pid = getpid();

    int lock_status = fcntl(customer_fd, F_SETLKW, &lock);
    if (lock_status == -1) {
        perror("Read lock\n");
        close(customer_fd);
        return false;
    }

    read_bytes = read(customer_fd, &customer, sizeof(struct Customer));
    if (read_bytes == -1) {
        perror("Reading file\n");
    }
    lock.l_type = F_UNLCK;
    fcntl(customer_fd, F_SETLK, &lock);

    sprintf(write_buffer, "%s%f\n%s", BALANCE, customer.balance, TO_PROCEED);
    write_bytes = write(client_socket, write_buffer, sizeof(write_buffer));
    if (write_bytes == -1) {
        perror("Write Balance to client\n");
        // close(customer_fd);
        return false;
    }
    if (write_bytes == 0) {
        printf("ok so what now\n");
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));

    return true;
}

bool change_balance(int client_socket, int id, int amount) {
    char read_buffer[1000], write_buffer[1000];
    int read_bytes, write_bytes;

    int customer_fd = open(CUSTOMER_FILE, O_RDWR);
    if (customer_fd == -1) {
        perror("Open customer file\n");
        return false;
    }

    off_t offset = lseek(customer_fd, id * sizeof(struct Customer), SEEK_SET);
    if (offset == -1) {
        write_bytes =
            write(client_socket, INVALID_USERID, strlen(INVALID_USERID));
        return false;
    }

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = id * sizeof(struct Customer);
    lock.l_len = sizeof(struct Customer);
    lock.l_pid = getpid();

    int lock_status = fcntl(customer_fd, F_SETLKW, &lock);
    if (lock_status == -1) {
        perror("Write lock\n");
        close(customer_fd);
        return false;
    }

    read_bytes = read(customer_fd, &customer, sizeof(struct Customer));
    if (read_bytes == -1) {
        perror("Reading file\n");
        close(customer_fd);
        return false;
    }

    int new_balance = customer.balance + amount;
    if (new_balance < 0) {
        sprintf(write_buffer, "%s\n%s", INSUFFICIENT_BALANCE, TO_PROCEED);
        write_bytes = write(client_socket, write_buffer, strlen(write_buffer));
        if (write_bytes == -1) {
            perror("Writing to customer about insufficient balance\n");
            close(customer_fd);
            return false;
        }
        // read(client_socket, read_buffer, sizeof(read_buffer));
        close(customer_fd);
        return false;
    }
    customer.balance = new_balance;

    offset = lseek(customer_fd, id * sizeof(struct Customer), SEEK_SET);
    if (offset == -1) {
        perror("lseek before write\n");
        close(customer_fd);
        return false;
    }

    write_bytes = write(customer_fd, &customer, sizeof(struct Customer));
    if (write_bytes == -1) {
        close(customer_fd);
        perror("Updating balance");
        return false;
    }

    lock.l_type = F_UNLCK;
    lock_status = fcntl(customer_fd, F_SETLK, &lock);
    if (lock_status == -1) {
        perror("Write lock\n");
        close(customer_fd);
        return false;
    }
}

bool deposit(int client_socket) {
    char read_buffer[1000], write_buffer[1000];
    int read_bytes, write_bytes;

    write_bytes = write(client_socket, DEPOSIT_AMOUNT, strlen(DEPOSIT_AMOUNT));
    if (write_bytes == -1) {
        perror("Writing asking for deposit amount");
        return false;
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Reading deposit amout");
        return false;
    }

    float amount = atof(read_buffer);
    int id = customer.account_number - 1;
    change_balance(client_socket, id, amount);
    view_balance(client_socket);
}

bool withdraw(int client_socket) {
    char read_buffer[1000], write_buffer[1000];
    int read_bytes, write_bytes;

    write_bytes =
        write(client_socket, WITHDRAW_AMOUNT, strlen(WITHDRAW_AMOUNT));
    if (write_bytes == -1) {
        perror("Writing asking for withdraw amount");
        return false;
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Reading withdraw amout");
        return false;
    }

    float amount = -1 * atof(read_buffer);
    int id = customer.account_number - 1;
    change_balance(client_socket, id, amount);
    view_balance(client_socket);
}

bool transfer_funds(int client_socket) {
    char read_buffer[1000], write_buffer[1000];
    int read_bytes, write_bytes;

    write_bytes =
        write(client_socket, WHOM_TO_TRANSFER, strlen(WHOM_TO_TRANSFER));
    if (write_bytes == -1) {
        perror("Asking Username of the person whom to transfer");
        return false;
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
    }
}

#endif