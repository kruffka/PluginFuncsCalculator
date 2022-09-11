#include <stdlib.h>
#include "../common.h"

func_info_t fun_info = {
    .func_name = "divide",
    .return_type = FLOAT,
    .number_of_args = 2
};

float divide(int argc, char *argv[]) {

    if (atoi(argv[1]) == 0) {
        // printf("division by zero\n");
        return 0.0;
    }

    return atoi(argv[0])/atoi(argv[1]);
}