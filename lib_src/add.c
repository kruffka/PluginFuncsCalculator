#include <stdlib.h>
#include "../common.h"

func_info_t fun_info = {
    .func_name = "add",
    .return_type = INT,
    .number_of_args = 2
};

// Any number of arguments can be summed here, but for simplicity, 
// the calculator will pass number_of_args to the function via a string
int add(int argc, char *argv[]) {

    int sum = 0;

    for (int i = 0; i < argc; i++) {
        sum += atoi(argv[i]);
    }

    return sum;
}