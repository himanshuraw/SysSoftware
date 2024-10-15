#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../Structures/constants.h"
#include "../Structures/customer.h"
#include "../Structures/employee.h"
#include "../Structures/transaction.h"

#ifndef COMMON
#define COMMON

bool create_administrator();
bool get_transaction_data(int transaction_id,
                          struct Transaction *ptr_transaction);
bool view_transactions(int client_socket, int account_number);
bool logout(int client_socket);

int init_semphore(int ID, char *file_name);
bool lock(struct sembuf *semOp, int sem_id);
bool unlock(struct sembuf *semOp, int sem_id);

int init_semphore(int ID, char *file_name) {
    key_t semKey = ftok(file_name, ID);
    union semun {
        int val;
    } semSet;

    int semctlStatus;
    int sem_id = semget(semKey, 1, 0);
    if (sem_id == -1) {
        sem_id = semget(semKey, 1, IPC_CREAT | 0700);
        if (sem_id == -1) {
            perror("Error while creating semaphore!");
            _exit(1);
        }

        semSet.val = 1;
        semctlStatus = semctl(sem_id, 0, SETVAL, semSet);
        if (semctlStatus == -1) {
            perror("Error while initializing a binary sempahore!");
            _exit(1);
        }
    }
    return sem_id;
}

bool lock(struct sembuf *semOp, int sem_id) {
    semOp->sem_flg = SEM_UNDO;
    semOp->sem_op = -1;
    semOp->sem_num = 0;
    int semopStatus = semop(sem_id, semOp, 1);
    if (semopStatus == -1) {
        perror("Error while locking critical section");
        return false;
    }
    return true;
}

bool unlock(struct sembuf *semOp, int sem_id) {
    semOp->sem_op = 1;
    int semopStatus = semop(sem_id, semOp, 1);
    if (semopStatus == -1) {
        perror("Error while operating on semaphore!");
        _exit(1);
    }
    return true;
}

bool exit_handler(int client_socket);

bool create_administrator() {
    int fd = open(EMPLOYEE_FILE, O_CREAT | O_WRONLY | O_APPEND, 0777);
    if (fd == -1) {
        perror("Openning file to create administrator\n");
        return false;
    }

    struct Employee administrator;

    if (lseek(fd, -sizeof(struct Employee), SEEK_END) >= 0) {
        return true;
    }

    char password[1000];
    strcpy(password, crypt(ADMINISTRATOR_PASSWORD, SALT));

    strcpy(administrator.name, ADMINISTRATOR_NAME);
    strcpy(administrator.username, ADMINISTRATOR_USERNAME);
    strcpy(administrator.password, password);
    administrator.age = 23;
    administrator.gender = 'M';
    administrator.id = 0;
    administrator.role = 2;

    if (write(fd, &administrator, sizeof(administrator)) == -1) {
        perror("Writing administrator to the file\n");
        return false;
    }
}

bool get_transaction_data(int transaction_id,
                          struct Transaction *ptr_transaction) {
    char read_buffer[1000], write_buffer[1000];
    int read_bytes, write_bytes;

    struct Transaction transaction;

    int transaction_fd = open(TRANSACTION_FILE, O_RDWR);
    if (transaction_fd == -1) {
        perror("Open transaction file\n");
        return false;
    }

    int id = transaction_id - 1;

    off_t offset =
        lseek(transaction_fd, id * sizeof(struct Transaction), SEEK_SET);
    if (offset == -1) {
        perror("Invalid ID\n");
        close(transaction_fd);
        return false;
    }

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = offset;
    lock.l_len = sizeof(struct Transaction);
    lock.l_pid = getpid();

    int lock_status = fcntl(transaction_fd, F_SETLKW, &lock);
    if (lock_status == -1) {
        perror("Read lock\n");
        close(transaction_fd);
        return false;
    }

    read_bytes = read(transaction_fd, &transaction, sizeof(struct Transaction));
    if (read_bytes == -1) {
        perror("Reading file\n");
        return -1;
    }

    *ptr_transaction = transaction;

    lock.l_type = F_UNLCK;
    fcntl(transaction_fd, F_SETLK, &lock);
    close(transaction_fd);
    return true;
}

bool view_transactions(int client_socket, int account_number) {
    char read_buffer[1000], write_buffer[1000];
    int read_bytes, write_bytes;
    struct Customer customer;
    int id = account_number - 1;

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
    lock.l_type = F_RDLCK;
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

    lock.l_type = F_UNLCK;
    lock_status = fcntl(customer_fd, F_SETLK, &lock);
    if (lock_status == -1) {
        perror("Write lock\n");
        close(customer_fd);
        return false;
    }

    strcpy(write_buffer, TRANSACTION_DETAIL);

    struct Transaction transaction;
    for (int i = 0; i < MAX_TRANSACTIONS; i++) {
        int index = customer.ptr - i - 1;
        if (index < 0) {
            index = MAX_TRANSACTIONS + index;
        }

        if (customer.transactions[index] == 0) {
            continue;
        }

        char operation[30];
        switch (transaction.operation) {
            case 0:
                strcpy(operation, "Deposited: + ");
                break;
            case 1:
                strcpy(operation, "Withdrawn: - ");
                break;
            case 2:
                strcpy(operation, "Someone Transfered: + ");
                break;
            case 3:
                strcpy(operation, "You Transfered: - ");
                break;

            default:
                break;
        }

        int num = i + 1;
        int transaction_id = customer.transactions[index];
        get_transaction_data(transaction_id, &transaction);
        char buffer[250];
        sprintf(buffer,
                "%d:\n\tTransaction id : %d\n\t%s\u20B9%f\n\tTime : %s\n\n",
                num, transaction_id, operation, transaction.amount,
                ctime(&transaction.time));
        strcat(write_buffer, buffer);
    }
    write_bytes = write(client_socket, write_buffer, sizeof(write_buffer));
    if (write_bytes == -1) {
        perror("Send transaction to client\n");
        return false;
    }

    read_bytes =
        read(client_socket, read_buffer, sizeof(read_buffer));  // Dummy read
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