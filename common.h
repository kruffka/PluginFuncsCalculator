#ifndef _COMMON_H_
#define _COMMON_H_

typedef enum return_type{
    INT = 0,
    FLOAT,
} return_type_e;

typedef struct func_info {
    char *func_name;
    return_type_e return_type;
    unsigned char number_of_args;
} func_info_t;

#endif // _COMMON_H_