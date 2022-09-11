#include "divide.h"

typedef struct func_info {
    char *func_name;
    char *return_type;
    unsigned char number_of_args;
} func_info_t;


func_info_t fun_info = {
    .func_name = "divide",
    .return_type = "float",
    .number_of_args = 2
};


float divide(int a, int b) {
    return a/b;
}