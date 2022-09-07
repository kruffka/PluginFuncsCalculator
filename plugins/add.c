#include "add.h"

typedef struct func_info {
    char *func_name;
    char *return_type;
    unsigned char number_of_args;
} func_info_t;


func_info_t fun_info = {
    .func_name = "add",
    .return_type = "int",
    .number_of_args = 2
};


int add(int a, int b) {
    return a + b;
}