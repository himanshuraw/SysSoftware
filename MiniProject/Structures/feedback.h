#include <stdbool.h>

#include "constants.h"
#ifndef FEEDBACK
#define FEEDBACK

struct Feedback {
    int id;
    bool reviewed;
    char text[FEEDBACK_SIZE];
};

#endif