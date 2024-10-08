// #include <sys/ipc.h>
// #include <sys/sem.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>

#include "../Structures/constants.h"
#include "../Structures/customer.h"
#include "../Structures/transaction.h"

#ifndef CUSTOMER
#define CUSTOMER

struct Customer customer;
// int sem_identifier;

bool customer_handler(int client_socket);
bool login_customer(int client_socket);
bool view_balance(int client_socket);
bool change_balance(int client_socket, int id, float amount, int operation);
bool deposit(int client_socket);
bool withdraw(int client_socket);
bool transfer_funds(int client_socket);
bool change_customer_password(int client_socket);
void write_transaction_to_array(int client_socket, int transaction_id, int id);
int write_transaction_to_file(int account_number, int operation, int amount);

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
            case 6:
                view_transactions(client_socket, customer.account_number);
                break;
            case 8:
                change_customer_password(client_socket);
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

    read_bytes =
        read(client_socket, read_buffer, sizeof(read_buffer));  // Dummy read

    return true;
}

bool change_balance(int client_socket, int id, float amount, int operation) {
    char read_buffer[1000], write_buffer[1000];
    int read_bytes, write_bytes;
    struct Customer local_customer;

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

    read_bytes = read(customer_fd, &local_customer, sizeof(struct Customer));
    if (read_bytes == -1) {
        perror("Reading file\n");
        close(customer_fd);
        return false;
    }

    int new_balance = local_customer.balance + amount;
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
    local_customer.balance = new_balance;

    offset = lseek(customer_fd, id * sizeof(struct Customer), SEEK_SET);
    if (offset == -1) {
        perror("lseek before write\n");
        close(customer_fd);
        return false;
    }

    write_bytes = write(customer_fd, &local_customer, sizeof(struct Customer));
    if (write_bytes == -1) {
        close(customer_fd);
        perror("Updating balance\n");
        return false;
    }

    lock.l_type = F_UNLCK;
    lock_status = fcntl(customer_fd, F_SETLK, &lock);
    if (lock_status == -1) {
        perror("Write lock\n");
        close(customer_fd);
        return false;
    }

    if (amount < 0) {
        amount *= -1;
    }
    int transaction_id = write_transaction_to_file(id + 1, operation, amount);
    write_transaction_to_array(client_socket, transaction_id, id);
    close(customer_fd);
    return true;
}

bool deposit(int client_socket) {
    char read_buffer[1000], write_buffer[1000];
    int read_bytes, write_bytes;

    write_bytes = write(client_socket, DEPOSIT_AMOUNT, strlen(DEPOSIT_AMOUNT));
    if (write_bytes == -1) {
        perror("Writing asking for deposit amount\n");
        return false;
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Reading deposit amout\n");
        return false;
    }

    float amount = atof(read_buffer);
    int id = customer.account_number - 1;
    change_balance(client_socket, id, amount, 0);
    view_balance(client_socket);
}

bool withdraw(int client_socket) {
    char read_buffer[1000], write_buffer[1000];
    int read_bytes, write_bytes;

    write_bytes =
        write(client_socket, WITHDRAW_AMOUNT, strlen(WITHDRAW_AMOUNT));
    if (write_bytes == -1) {
        perror("Writing asking for withdraw amount\n");
        return false;
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Reading withdraw amout\n");
        return false;
    }

    float amount = -1 * atof(read_buffer);
    int id = customer.account_number - 1;
    change_balance(client_socket, id, amount, 1);
    view_balance(client_socket);
}

bool transfer_funds(int client_socket) {
    char read_buffer[1000], write_buffer[1000], buffer[100];
    int read_bytes, write_bytes;

    int to_id;
    int from_id = customer.account_number - 1;

    write_bytes =
        write(client_socket, WHOM_TO_TRANSFER, strlen(WHOM_TO_TRANSFER));
    if (write_bytes == -1) {
        perror("Asking Username of the person whom to transfer\n");
        return false;
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Reading Username of the person whom to transfer\n");
        return false;
    }

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, read_buffer);
    strtok(buffer, "-");
    to_id = atoi(strtok(NULL, "-")) - 1;

    write_bytes =
        write(client_socket, TRANSFER_AMOUNT, strlen(TRANSFER_AMOUNT));
    if (write_bytes == -1) {
        perror("Write to ask amount to be transfered\n");
        return false;
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Read amount to be transfered\n");
        return false;
    }

    float amount = -1 * atof(read_buffer);

    if (!change_balance(client_socket, from_id, amount, 3)) {
        perror("Unable to deduct amount\n");
        return false;
    }
    amount *= -1;
    change_balance(client_socket, to_id, amount, 2);
    return true;
}

bool change_customer_password(int client_socket) {
    char read_buffer[1000], write_buffer[1000], buffer[100];
    int read_bytes, write_bytes;

    write_bytes = write(client_socket, NEW_PASSWORD, strlen(NEW_PASSWORD));
    if (write_bytes == -1) {
        perror("Ask for new Password\n");
        return false;
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Reading new Password\n");
        return false;
    }

    char password[100];
    strcpy(password, read_buffer);
    char hashed_password[1000];
    strcpy(hashed_password, crypt(read_buffer, SALT));

    int id = customer.account_number - 1;

    int customer_fd = open(CUSTOMER_FILE, O_RDWR);
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

    strcpy(customer.password, hashed_password);
    offset = lseek(customer_fd, id * sizeof(struct Customer), SEEK_SET);
    if (offset == -1) {
        write_bytes =
            write(client_socket, INVALID_USERID, strlen(INVALID_USERID));
        return false;
    }

    write_bytes = write(customer_fd, &customer, sizeof(struct Customer));
    if (write_bytes == -1) {
        perror("Writing updated password to the file\n");
        close(customer_fd);
        return false;
    }

    lock.l_type = F_UNLCK;
    fcntl(customer_fd, F_SETLK, &lock);

    sprintf(write_buffer, "%s\nYour new password is: %s", SUCCESS, password);
    write_bytes = write(client_socket, write_buffer, sizeof(write_buffer));
    if (write_bytes == -1) {
        perror("Success message to client\n");
        close(customer_fd);
        return false;
    }
    close(customer_fd);

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    return true;
}

int write_transaction_to_file(int account_number, int operation, int amount) {
    struct Transaction new_transaction, prev_transaction;
    new_transaction.account_number = account_number;
    new_transaction.amount = amount;
    new_transaction.operation = operation;
    new_transaction.time = time(NULL);

    int transaction_fd =
        open(TRANSACTION_FILE, O_CREAT | O_APPEND | O_RDWR, 0777);
    if (transaction_fd == -1) {
        perror("Open transaction file\n");
        return -1;
    }

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_pid = getpid();

    int lock_status;

    off_t offset = lseek(transaction_fd, -sizeof(struct Transaction), SEEK_END);
    if (offset >= 0) {
        lock.l_start = offset;
        lock.l_len = 2 * sizeof(struct Transaction);

        lock_status = fcntl(transaction_fd, F_SETLKW, &lock);
        if (lock_status == -1) {
            perror("Read lock\n");
            close(transaction_fd);
            return false;
        }

        int read_bytes =
            read(transaction_fd, &prev_transaction, sizeof(struct Transaction));
        if (read_bytes == -1) {
            perror("Reading file\n");
            return -1;
        }

        new_transaction.id = prev_transaction.id + 1;
    } else {
        lock.l_len = sizeof(struct Transaction);
        lock_status = fcntl(transaction_fd, F_SETLKW, &lock);
        if (lock_status == -1) {
            perror("Read lock\n");
            close(transaction_fd);
            return false;
        }

        new_transaction.id = 1;
    }

    offset = lseek(transaction_fd, 0, SEEK_END);
    int write_bytes =
        write(transaction_fd, &new_transaction, sizeof(struct Transaction));
    if (write_bytes == -1) {
        perror("Write to transaction file\n");
        close(transaction_fd);
        return false;
    }

    lock.l_type = F_UNLCK;
    fcntl(transaction_fd, F_SETLK, &lock);
    close(transaction_fd);
    return new_transaction.id;
}

void write_transaction_to_array(int client_socket, int transaction_id, int id) {
    char read_buffer[1000], write_buffer[1000];
    int read_bytes, write_bytes;
    struct Customer local_customer;

    int customer_fd = open(CUSTOMER_FILE, O_RDWR);
    if (customer_fd == -1) {
        perror("Open customer file\n");
        return;
    }

    off_t offset = lseek(customer_fd, id * sizeof(struct Customer), SEEK_SET);
    if (offset == -1) {
        write_bytes =
            write(client_socket, INVALID_USERID, strlen(INVALID_USERID));
        return;
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
        return;
    }

    read_bytes = read(customer_fd, &local_customer, sizeof(struct Customer));
    if (read_bytes == -1) {
        perror("Reading file\n");
        close(customer_fd);
        return;
    }
    local_customer.transactions[local_customer.ptr] = transaction_id;
    local_customer.ptr = (local_customer.ptr + 1) % MAX_TRANSACTIONS;

    offset = lseek(customer_fd, id * sizeof(struct Customer), SEEK_SET);
    if (offset == -1) {
        write_bytes =
            write(client_socket, INVALID_USERID, strlen(INVALID_USERID));
        return;
    }

    write_bytes = write(customer_fd, &local_customer, sizeof(struct Customer));
    if (write_bytes == -1) {
        perror("Write Transaction to customer file\n");
        close(customer_fd);
        return;
    }

    lock.l_type = F_UNLCK;
    lock_status = fcntl(customer_fd, F_SETLK, &lock);
    if (lock_status == -1) {
        perror("Write lock\n");
        close(customer_fd);
        return;
    }
    close(customer_fd);
}

#endif