#include <stdlib.h>
#include "../common.h"

func_info_t fun_info = {
    .func_name = "mult",
    .return_type = INT,
    .number_of_args = 2
};

int mult(int argc, char *argv[]) {

    return atoi(argv[0])*atoi(argv[1]);
}