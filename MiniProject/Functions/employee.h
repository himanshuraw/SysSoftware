#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../Structures/customer.h"
#include "../Structures/employee.h"
#include "administrator.h"
#include "manager.h"

#ifndef EMPLOYEE
#define EMPLOYEE

struct Employee employee;

bool employee_handler(int client_socket, int role);
int login_employee(int client_socket, int role);

bool bank_employee_menu(int client_socket);
bool manager_menu(int client_socket);
bool administrator_menu(int client_socket);

int add_customer(int client_socket);
bool modify_customer_details(int client_socket);
bool change_password(int client_socket);
bool view_customer_transactions(int client_socket);
bool view_assigned_loans(int client_socket);

bool employee_handler(int client_socket, int role) {
    int real_role = login_employee(client_socket, role);
    if (real_role != role) {
        return false;
    }
    sem_id = init_semphore(customer.account_number, CUSTOMER_FILE);
    lock(&sem_operation, sem_id);
    bool flag = true;
    while (flag) {
        switch (role) {
            case 0:
                flag = bank_employee_menu(client_socket);
                break;

            case 1:
                flag = manager_menu(client_socket);
                break;

            case 2:
                flag = administrator_menu(client_socket);
                break;
            default:
                unlock(&sem_operation, sem_id);
                break;
        }
    }
}

int login_employee(int client_socket, int role) {
    char read_buffer[1000], write_buffer[1000], buffer[1000];
    int read_bytes, write_bytes;
    int ID;

    memset(read_buffer, 0, sizeof(read_buffer));
    memset(write_buffer, 0, sizeof(write_buffer));

    role == 0   ? strcpy(write_buffer, EMPLOYEE_LOGIN_PAGE)
    : role == 1 ? strcpy(write_buffer, MANAGER_LOGIN_PAGE)
                : strcpy(write_buffer, ADMINISTRATOR_LOGIN_PAGE);
    strcat(write_buffer, USERNAME);

    write_bytes = write(client_socket, write_buffer, strlen(write_buffer));
    if (write_bytes == -1) {
        perror("Write login page");
        return -1;
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Read Username");
        return -1;
    }

    if (read_bytes == 0) {
        perror("No input");
        return -1;
    }

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, read_buffer);
    strtok(buffer, "-");
    ID = atoi(strtok(NULL, "-"));

    int file_fd = open(EMPLOYEE_FILE, O_RDONLY);
    if (file_fd == -1) {
        perror("Open employee file");
        return -1;
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
        return -1;
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
        return -1;
    }

    close(file_fd);

    write_bytes = write(client_socket, PASSWORD, strlen(PASSWORD));
    if (write_bytes == -1) {
        perror("Writing password in employee\n");
        return -1;
    }

    memset(read_buffer, 0, sizeof(read_buffer));
    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Read password in employee\n");
        return -1;
    }

    char hashed_password[1000];
    strcpy(hashed_password, crypt(read_buffer, SALT));

    if (strcmp(hashed_password, employee.password) != 0) {
        write_bytes =
            write(client_socket, INVALID_PASSWORD, strlen(INVALID_PASSWORD));
        return -1;
    }

    return employee.role;
}

bool bank_employee_menu(int client_socket) {
    char read_buffer[1000], write_buffer[1000];
    int read_bytes, write_bytes;
    write_bytes = write(client_socket, EMPLOYEE_MENU, strlen(EMPLOYEE_MENU));
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
        case 2:
            modify_customer_details(client_socket);
            break;
        case 3:
            view_assigned_loans(client_socket);
            break;
        case 4:
            view_customer_transactions(client_socket);
            break;
        case 5:
            change_password(client_socket);
            break;
        default:
            unlock(&sem_operation, sem_id);
            return false;
    }
}

bool manager_menu(int client_socket) {
    char read_buffer[1000], write_buffer[1000];
    int read_bytes, write_bytes;
    write_bytes = write(client_socket, MANAGER_MENU, strlen(MANAGER_MENU));
    if (write_bytes == -1) {
        perror("Writing manager menu\n");
        return false;
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Reading choice from client");
    }

    int choice = atoi(read_buffer);
    switch (choice) {
        case 1:
            toggle_customer_account(client_socket, true);
            break;
        case 2:
            toggle_customer_account(client_socket, false);
            break;
        case 3:
            assign_loan(client_socket);
            break;
        case 4:
            review_feedbacks(client_socket);
            break;
        case 5:
            change_password(client_socket);
            break;
        default:
            unlock(&sem_operation, sem_id);
            return false;
    }
}

bool administrator_menu(int client_socket) {
    char read_buffer[1000], write_buffer[1000];
    int read_bytes, write_bytes;
    write_bytes =
        write(client_socket, ADMINISTRATOR_MENU, strlen(ADMINISTRATOR_MENU));
    if (write_bytes == -1) {
        perror("Writing administrator menu\n");
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
        case 2:
            modify_customer_details(client_socket);
            break;
        case 3:
            modify_employee_details(client_socket);
            break;
        case 4:
            manage_role(client_socket);
            break;
        case 5:
            change_password(client_socket);
            break;
        default:
            unlock(&sem_operation, sem_id);
            return false;
    }
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

    // Pointer to next empty space in transaction history
    new_customer.ptr = 0;

    // Transaction
    for (int i = 0; i < MAX_TRANSACTIONS; i++) {
        new_customer.transactions[i] = 0;
    }

    // Loan
    new_customer.loan = -1;

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
    // TODO: if DNE then create the file
    // TODO: lseek.END == 0 --> accno = 1
    // TODO: find prev and accno = prev+1
    //     ! SEE TRANSACTION TO FILE
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

bool modify_customer_details(int client_socket) {
    char read_buffer[1000], write_buffer[1000], buffer[100];
    int read_bytes, write_bytes;
    int ID;
    struct Customer customer;

    write_bytes = write(client_socket, WHICH_USER, strlen(WHICH_USER));
    if (write_bytes == -1) {
        perror("Ask for username\n");
        return false;
    }

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    if (read_bytes == -1) {
        perror("Reading username\n");
        return false;
    }

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, read_buffer);
    strtok(buffer, "-");
    ID = atoi(strtok(NULL, "-")) - 1;

    while (true) {
        write_bytes = write(client_socket, WHICH_DETAIL, strlen(WHICH_DETAIL));
        if (write_bytes == -1) {
            perror("Ask for detail to modify\n");
            return false;
        }

        read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
        if (read_bytes == -1) {
            perror("Reading detail\n");
            return false;
        }
        int choice = atoi(read_buffer);

        switch (choice) {
            case 1:
                write_bytes = write(client_socket, ASK_NAME, strlen(ASK_NAME));
                break;
            case 2:
                write_bytes = write(client_socket, ASK_AGE, strlen(ASK_AGE));
                break;
            case 3:
                write_bytes =
                    write(client_socket, ASK_GENDER, strlen(ASK_GENDER));
                break;
            default:
                return true;
        }

        if (write_bytes == -1) {
            perror("Ask for new detail\n");
            return false;
        }

        read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
        if (read_bytes == -1) {
            perror("Reading new detail\n");
            return false;
        }

        strcpy(buffer, read_buffer);

        int customer_fd = open(CUSTOMER_FILE, O_RDWR);
        off_t offset =
            lseek(customer_fd, ID * sizeof(struct Customer), SEEK_SET);
        if (offset == -1) {
            write_bytes =
                write(client_socket, INVALID_USERID, strlen(INVALID_USERID));
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

        switch (choice) {
            case 1:
                strcpy(customer.name, buffer);
                strcpy(customer.username, buffer);
                strcat(customer.username, "-");
                memset(buffer, 0, sizeof(buffer));
                sprintf(buffer, "%d", customer.account_number);
                strcat(customer.username, buffer);
                break;
            case 2:
                int age = atoi(buffer);
                customer.age = age;
                break;
            case 3:
                char gender = buffer[0];
                customer.gender = gender;
                break;
            default:
                close(customer_fd);
                return true;
        }

        offset = lseek(customer_fd, ID * sizeof(struct Customer), SEEK_SET);
        if (offset == -1) {
            write_bytes =
                write(client_socket, INVALID_USERID, strlen(INVALID_USERID));
            return false;
        }
        write_bytes = write(customer_fd, &customer, sizeof(struct Customer));
        if (write_bytes == -1) {
            perror("Writing updated name and username to the file\n");
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

        close(customer_fd);
    }
}

bool change_password(int client_socket) {
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

    int id = employee.id;

    int employee_fd = open(EMPLOYEE_FILE, O_RDWR);
    off_t offset = lseek(employee_fd, id * sizeof(struct Employee), SEEK_SET);
    if (offset == -1) {
        write_bytes =
            write(client_socket, INVALID_USERID, strlen(INVALID_USERID));
        return false;
    }

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = id * sizeof(struct Employee);
    lock.l_len = sizeof(struct Employee);
    lock.l_pid = getpid();

    int lock_status = fcntl(employee_fd, F_SETLKW, &lock);
    if (lock_status == -1) {
        perror("Write lock\n");
        close(employee_fd);
        return false;
    }

    read_bytes = read(employee_fd, &employee, sizeof(struct Employee));
    if (read_bytes == -1) {
        perror("Reading file\n");
        close(employee_fd);
        return false;
    }

    strcpy(employee.password, hashed_password);
    offset = lseek(employee_fd, id * sizeof(struct Employee), SEEK_SET);
    if (offset == -1) {
        write_bytes =
            write(client_socket, INVALID_USERID, strlen(INVALID_USERID));
        return false;
    }

    write_bytes = write(employee_fd, &employee, sizeof(struct Employee));
    if (write_bytes == -1) {
        perror("Writing updated password to the file\n");
        close(employee_fd);
        return false;
    }

    lock.l_type = F_UNLCK;
    fcntl(employee_fd, F_SETLK, &lock);

    sprintf(write_buffer, "%s\nYour new password is: %s", SUCCESS, password);
    write_bytes = write(client_socket, write_buffer, sizeof(write_buffer));
    if (write_bytes == -1) {
        perror("Success message to client\n");
        close(employee_fd);
        return false;
    }
    close(employee_fd);

    read_bytes = read(client_socket, read_buffer, sizeof(read_buffer));
    return true;
}

bool view_customer_transactions(int client_socket) {
    char read_buffer[1000], write_buffer[1000], buffer[1000];
    int read_bytes, write_bytes;
    int account_number;

    write_bytes = write(client_socket, WHICH_USER, strlen(WHICH_USER));
    if (write_bytes == -1) {
        perror("Asking Username of the\n");
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
    account_number = atoi(strtok(NULL, "-"));

    view_transactions(client_socket, account_number);
}

bool view_assigned_loans(int client_socket) {
    char read_buffer[1000], write_buffer[1000], buffer[900];
    int read_bytes, write_bytes;

    int employee_fd = open(EMPLOYEE_FILE, O_RDWR);
    if (employee_fd == -1) {
        perror("Open employee file");
        return false;
    }

    int emp_id = employee.id;

    off_t offset =
        lseek(employee_fd, emp_id * sizeof(struct Employee), SEEK_SET);

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = offset;
    lock.l_len = sizeof(struct Employee);
    lock.l_pid = getpid();

    if (fcntl(employee_fd, F_SETLKW, &lock) == -1) {
        perror("Read lock on employee\n");
        close(employee_fd);
        return false;
    }

    if (read(employee_fd, &employee, sizeof(employee)) == -1) {
        perror("Reading from employee file\n");
        close(employee_fd);
        return false;
    }

    int loan_fd = open(LOAN_FILE, O_RDWR);

    if (loan_fd == -1) {
        memset(write_buffer, 0, sizeof(write_buffer));
        sprintf(write_buffer, "No new loan assigned\n");
        if (write(client_socket, write_buffer, sizeof(write_buffer)) == -1) {
            perror("Writing to client\n");
            close(employee_fd);
            return false;
        }

        read(client_socket, read_buffer, sizeof(read_buffer));

        perror("Opening loan file\n");
        close(employee_fd);
        return true;
    }
    struct Loan loan;

    // No need of lock as this portion will not be accessed by anyone else
    strcpy(write_buffer, "ASSIGNED LOANS\n");
    for (int i = 0; i < LOAN_SIZE; i++) {
        int loan_id = employee.loans[i] - 1;
        if (loan_id < 0) {
            continue;
        }
        off_t loan_offset =
            lseek(loan_fd, loan_id * sizeof(struct Loan), SEEK_SET);
        read_bytes = read(loan_fd, &loan, sizeof(struct Loan));
        if (read_bytes == -1) {
            perror("Reading loan file\n");
            close(loan_fd);
            close(employee_fd);
            return false;
        }

        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer,
                "Loan ID: %d\nCustomer account number: %d\tAmount: %ld\n\n",
                loan.id, loan.customer_account_number, loan.amount);
        strcat(write_buffer, buffer);
    }
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "Enter Loan ID to Accept or Reject");
    strcat(write_buffer, buffer);
    if (write(client_socket, write_buffer, sizeof(write_buffer)) == -1) {
        perror("Writing loans to client\n");
        close(loan_fd);
        close(employee_fd);
        return false;
    }

    if (read(client_socket, read_buffer, sizeof(read_buffer)) == -1) {
        perror("Reading loans ID from client\n");
        close(loan_fd);
        close(employee_fd);
        return false;
    }

    int ID = atoi(read_buffer) - 1;
    if (lseek(loan_fd, ID * sizeof(struct Loan), SEEK_SET) == -1) {
        perror("lseek loan\n");
        close(loan_fd);
        close(employee_fd);
        return false;
    }
    if (read(loan_fd, &loan, sizeof(struct Loan)) == -1) {
        perror("Read loan\n");
        close(loan_fd);
        close(employee_fd);
        return false;
    }

    memset(write_buffer, 0, sizeof(write_buffer));
    sprintf(write_buffer, "Choose\n1: Reject\t2:Accept");
    if (write(client_socket, write_buffer, sizeof(write_buffer)) == -1) {
        perror("Writing loans to client\n");
        close(loan_fd);
        close(employee_fd);
        return false;
    }

    memset(read_buffer, 0, sizeof(read_buffer));
    if (read(client_socket, read_buffer, sizeof(read_buffer)) == -1) {
        perror("Reading loans ID from client\n");
        close(loan_fd);
        close(employee_fd);
        return false;
    }
    int choice = atoi(read_buffer);
    loan.status = choice - 1;

    if (lseek(loan_fd, ID * sizeof(struct Loan), SEEK_SET) == -1) {
        perror("lseek loan\n");
        close(loan_fd);
        close(employee_fd);
        return false;
    }

    if (write(loan_fd, &loan, sizeof(struct Loan)) == -1) {
        perror("Writing into loan file\n");
        close(loan_fd);
        close(employee_fd);
        return false;
    }

    close(loan_fd);

    int i = 0;
    for (; i < LOAN_SIZE; i++) {
        if (employee.loans[i] == ID + 1) {
            break;
        }
    }

    employee.loans[i] = employee.loans[employee.ptr];
    employee.loans[employee.ptr] = -1;

    lseek(employee_fd, emp_id * sizeof(struct Employee), SEEK_SET);

    if (write(employee_fd, &employee, sizeof(struct Employee)) == -1) {
        perror("Writing into employee file\n");
        close(employee_fd);
        return false;
    }

    close(employee_fd);
    if (write(client_socket, SUCCESS, sizeof(SUCCESS)) == -1) {
        return false;
    }
    read(client_socket, read_buffer, sizeof(read_buffer));
    return true;
}
#endif