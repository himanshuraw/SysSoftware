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
bool review_feedbacks(int client_socket);

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
    struct Loan loan;
    struct Employee employee;

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

            i == 0 ? strcpy(write_buffer, buffer)
                   : strcat(write_buffer, buffer);

            i++;
        }
    } while (read_bytes && strlen(buffer) < 900);

    if (i == 0) {
        memset(write_buffer, 0, sizeof(write_buffer));
        sprintf(write_buffer, "No new loan to assign\n");
        if (write(client_socket, write_buffer, sizeof(write_buffer)) == -1) {
            perror("Writing to client\n");
            close(loan_fd);
            return false;
        }

        read(client_socket, read_buffer, sizeof(read_buffer));
        return true;
    }

    strcat(write_buffer, ASK_LOAN_ID);
    write_bytes = write(client_socket, write_buffer, sizeof(write_buffer));
    if (write_bytes == -1) {
        perror("Writing to client\n");
        close(loan_fd);
        return false;
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Read from client\n");
        close(loan_fd);
        return false;
    }

    int ID = atoi(read_buffer) - 1;
    off_t offset = lseek(loan_fd, ID * sizeof(struct Loan), SEEK_SET);

    lock.l_type = F_WRLCK;
    lock.l_start = offset;
    lock.l_len = sizeof(struct Loan);

    lock_status = fcntl(loan_fd, F_SETLKW, &lock);
    if (lock_status == -1) {
        perror("Write lock\n");
        close(loan_fd);
        return false;
    }

    read_bytes = read(loan_fd, &loan, sizeof(struct Loan));
    if (read_bytes == -1) {
        perror("Read from Loan file\n");
        close(loan_fd);
        return false;
    }

    loan.assigned = true;

    write_bytes =
        write(client_socket, ASK_EMPLOYEE_ID, sizeof(ASK_EMPLOYEE_ID));
    if (write_bytes == -1) {
        perror("Writing to client\n");
        close(loan_fd);
        return false;
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Read from client\n");
        close(loan_fd);
        return false;
    }

    int emp_id = atoi(read_buffer);

    int employee_fd = open(EMPLOYEE_FILE, O_RDWR);
    if (employee_fd == -1) {
        perror("Open employee\n");
        close(loan_fd);
        return false;
    }

    off_t emp_offset =
        lseek(employee_fd, emp_id * sizeof(struct Employee), SEEK_SET);

    struct flock emp_lock;
    emp_lock.l_type = F_WRLCK;
    emp_lock.l_len = sizeof(struct Employee);
    emp_lock.l_start = emp_offset;
    emp_lock.l_whence = SEEK_SET;
    emp_lock.l_pid = getpid();

    if (fcntl(employee_fd, F_SETLKW, &emp_lock)) {
        perror("Write lock");
        close(employee_fd);
        close(loan_fd);
        return false;
    }

    read_bytes = read(employee_fd, &employee, sizeof(struct Employee));
    if (read_bytes == -1) {
        perror("Read from employee file");
        close(employee_fd);
        close(loan_fd);
        return false;
    }

    // If it is not an employee
    if (employee.role != 0) {
        memset(write_buffer, 0, sizeof(write_buffer));

        sprintf(write_buffer, "Given id is of %s",
                employee.role == 1 ? "another Manager" : "an Administrator");
        write_bytes = write(client_socket, write_buffer, sizeof(write_buffer));
        if (write_bytes == -1) {
            perror("Write to client");
            close(employee_fd);
            close(loan_fd);
            return false;
        }

        read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
        close(employee_fd);
        close(loan_fd);
        return false;
    }

    // If at max capacity is reached
    if (employee.loans[employee.ptr % LOAN_SIZE] != -1) {
        write_bytes = write(client_socket, MAX_REACHED, strlen(MAX_REACHED));
        if (write_bytes == -1) {
            perror("Write to client");
            close(employee_fd);
            close(loan_fd);
            return false;
        }

        read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
        close(employee_fd);
        close(loan_fd);
        return false;
    }
    employee.loans[(employee.ptr) % LOAN_SIZE] = ID + 1;
    employee.ptr += 1;

    if (lseek(employee_fd, emp_offset, SEEK_SET) == -1) {
        perror("lseek in employee file");
        close(employee_fd);
        close(loan_fd);
        return false;
    }

    write_bytes = write(employee_fd, &employee, sizeof(struct Employee));
    if (write_bytes == -1) {
        perror("Writing to employee file");
        close(employee_fd);
        close(loan_fd);
        return false;
    }

    emp_lock.l_type = F_UNLCK;
    if (fcntl(employee_fd, F_SETLK, &emp_lock) == -1) {
        perror("Write lock employee\n");
        close(employee_fd);
        close(loan_fd);
        return false;
    }

    close(employee_fd);

    if (lseek(loan_fd, offset, SEEK_SET) == -1) {
        perror("lseek in loan file");
        close(loan_fd);
        return false;
    }

    write_bytes = write(loan_fd, &loan, sizeof(struct Loan));
    if (write_bytes == -1) {
        perror("Writing to loan file");
        close(loan_fd);
        return false;
    }

    lock.l_type = F_UNLCK;
    lock_status = fcntl(loan_fd, F_SETLK, &lock);
    if (lock_status == -1) {
        perror("Write lock\n");
        close(loan_fd);
        return false;
    }
    close(loan_fd);

    if (write(client_socket, SUCCESS, strlen(SUCCESS)) == -1) {
        perror("Writing to client\n");
        return false;
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    return true;
}

bool review_feedbacks(int client_socket) {
    char read_buffer[1000], write_buffer[1000], buffer[1000];
    int read_bytes, write_bytes;
    struct Feedback feedback;

    int feedback_fd = open(FEEDBACK_FILE, O_RDWR);
    if (feedback_fd == -1) {
        memset(write_buffer, 0, sizeof(write_buffer));
        sprintf(write_buffer, "No new feedback to review\n");
        if (write(client_socket, write_buffer, sizeof(write_buffer)) == -1) {
            perror("Writing to client\n");
            close(feedback_fd);
            return false;
        }

        read(client_socket, read_buffer, sizeof(read_buffer));
        return true;
    }
    int i = 0;
    do {
        read_bytes = read(feedback_fd, &feedback, sizeof(struct Feedback));
        if (read_bytes == -1) {
            perror("Reading file\n");
            close(feedback_fd);
            return false;
        }
        if (read_bytes == 0) break;
        if (!feedback.reviewed) {
            memset(buffer, 0, sizeof(buffer));
            sprintf(buffer, "ID %d : %s\n", feedback.id, feedback.text);
            if (i == 0) {
                strcpy(write_buffer, buffer);
            } else {
                strcat(write_buffer, buffer);
            }
            i++;
        }
    } while (read_bytes && strlen(buffer) < 850);
    if (i == 0) {
        memset(write_buffer, 0, sizeof(write_buffer));
        sprintf(write_buffer, "No new feedback to review\n");
        if (write(client_socket, write_buffer, sizeof(write_buffer)) == -1) {
            perror("Writing to client\n");
            close(feedback_fd);
            return false;
        }

        read(client_socket, read_buffer, sizeof(read_buffer));
        return true;
    }

    strcat(write_buffer, "Enter the ID of Feedback to mark it as reviewed");
    write_bytes = write(client_socket, write_buffer, sizeof(write_buffer));
    if (write_bytes == -1) {
        perror("Writing to client\n");
        close(feedback_fd);
        return false;
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Read from client\n");
        close(feedback_fd);
        return false;
    }

    int ID = atoi(read_buffer);
    off_t offset = lseek(feedback_fd, ID * sizeof(struct Feedback), SEEK_SET);
    read_bytes = read(feedback_fd, &feedback, sizeof(struct Feedback));
    if (read_bytes == -1) {
        perror("Read from Feedback file\n");
        close(feedback_fd);
        return false;
    }

    feedback.reviewed = true;

    lseek(feedback_fd, ID * sizeof(struct Feedback), SEEK_SET);

    if (write(feedback_fd, &feedback, sizeof(struct Feedback)) == -1) {
        perror("Write to Feedback file");
        close(feedback_fd);
        return false;
    }

    close(feedback_fd);

    if (write(client_socket, SUCCESS, strlen(SUCCESS)) == -1) {
        perror("Writing to client\n");
        close(feedback_fd);
        return false;
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    return true;
}
#endif