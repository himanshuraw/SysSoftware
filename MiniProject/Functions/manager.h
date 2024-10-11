#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../Structures/customer.h"

#ifndef MANAGER
#define MANAGER

bool toggle_customer_account(int client_socket, bool activate);
bool assign_loan(int client_socket);

bool toggle_customer_account(int client_socket, bool activate) {
    char read_buffer[1000], write_buffer[1000], buffer[1000];
    int read_bytes, write_bytes;
    int ID;
    struct Customer customer;

    write_bytes = write(client_socket, WHICH_USER, strlen(WHICH_USER));
    if (write_bytes == -1) {
        perror("Ask for username\n");
        return false;
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (write_bytes == -1) {
        perror("Read username\n");
        return false;
    }

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, read_buffer);
    strtok(buffer, "-");
    ID = atoi(strtok(NULL, "-")) - 1;

    int customer_fd = open(CUSTOMER_FILE, O_RDWR);

    off_t offset = lseek(customer_fd, ID * sizeof(struct Customer), SEEK_SET);
    if (offset == -1) {
        perror("Lseek to customer");
        return false;
    }

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = ID * sizeof(struct Customer);
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

    customer.is_active = activate;

    offset = lseek(customer_fd, ID * sizeof(struct Customer), SEEK_SET);
    if (offset == -1) {
        perror("Lseek to customer");
        return false;
    }

    write_bytes = write(customer_fd, &customer, sizeof(struct Customer));
    if (write_bytes == -1) {
        perror("Writing to the file\n");
        close(customer_fd);
        return false;
    }

    lock.l_type = F_UNLCK;
    lock_status = fcntl(customer_fd, F_SETLK, &lock);
    if (lock_status == -1) {
        perror("unlock write-lock from customer file");
        close(customer_fd);
        return -1;
    }

    write_bytes = write(client_socket, SUCCESS, sizeof(SUCCESS));
    if (write_bytes == -1) {
        perror("write success message\n");
        close(customer_fd);
        return false;
    }

    read_bytes =
        read(client_socket, read_buffer, sizeof(read_buffer));  // Dummy read;
    close(customer_fd);
    return true;
}

bool assign_loan(int client_socket) {
    char read_buffer[1000], write_buffer[1000], buffer[1000];
    int read_bytes, write_bytes;
    int loan_fd = open(LOAN_FILE, O_RDWR);

    if (loan_fd == -1) {
        perror("Open loan file\n");
        return false;
    }

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_pid = getpid();

    int lock_status = fcntl(loan_fd, F_SETLKW, &lock);
    if (lock_status == -1) {
        perror("Write lock\n");
        close(loan_fd);
        return false;
    }

    int i = 0;
    do {
        struct Loan loan;
        read_bytes = read(loan_fd, &loan, sizeof(struct Loan));
        if (read_bytes == -1) {
            perror("Reading file\n");
            close(loan_fd);
            return false;
        }
        if (read_bytes == 0) break;
        if (!loan.assigned) {
            memset(buffer, 0, sizeof(buffer));
            sprintf(buffer,
                    "Loan ID: %d\nCustomer account number: %d\tAmount: %ld\n\n",
                    loan.id, loan.customer_account_number, loan.amount);
            if (i == 0) {
                strcpy(write_buffer, buffer);
            } else {
                strcat(write_buffer, buffer);
            }
            i++;
        }
    } while (read_bytes && strlen(buffer) < 945);

    write_bytes = write(client_socket, write_buffer, sizeof(write_buffer));
    if (write_bytes == -1) {
        perror("Writting to client");
        close(loan_fd);
        return false;
    }

    // TODO : Assign to employee :: make different lock for it
    // TODO : Make loan.assigned = true :: change rdlock -> wrlock for LoanID

    lock.l_type = F_UNLCK;
    lock_status = fcntl(loan_fd, F_SETLK, &lock);
    if (lock_status == -1) {
        perror("Write lock\n");
        close(loan_fd);
        return false;
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    close(loan_fd);
    return true;
}

#endif