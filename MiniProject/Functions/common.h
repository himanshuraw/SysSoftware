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

void error_handler();

void error_handler() { printf("Invalid"); }

#endif