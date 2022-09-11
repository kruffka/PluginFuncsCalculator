#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <assert.h>
#include <dirent.h>
#include <string.h>
#include <malloc.h>
#include "common.h"

typedef struct plugins_info {
    // pointer to global struct func_info in lib
    func_info_t *fun_info;
    // pointer to handle libs
    void *lib_p;
    // function pointer to function in lib
    union func_p {
        int (*intfun_p)();
        float (*floatfun_p)();
    } func_p;
    
} plugins_info_t;

#define AssertFatal(cond, fmt, ...) do { if (!cond) { printf(fmt, __VA_ARGS__); assert(cond); } } while(0)

int plugin_load(plugins_info_t *plugins, struct dirent **direntP, int num_of_pl) {
    int libs_loaded = 0;

    for (int i = 0; i < num_of_pl; i++) {

        printf("[%d/%d] Loading library %s\n", i+1, num_of_pl, direntP[i]->d_name);

        plugins[i].lib_p = dlopen(direntP[i]->d_name, RTLD_LAZY);
        AssertFatal(plugins[i].lib_p != 0, "Error loading library %s: %s\n", direntP[i]->d_name, dlerror());

        plugins[i].fun_info = (func_info_t *)dlsym(plugins[i].lib_p, "fun_info");
        AssertFatal(plugins[i].fun_info != 0, "Error getting func_info in %s: %s\n", direntP[i]->d_name, dlerror());


        switch (plugins[i].fun_info->return_type)
        {
        case INT:
            plugins[i].func_p.intfun_p = dlsym(plugins[i].lib_p, plugins[i].fun_info->func_name);
            AssertFatal(plugins[i].func_p.intfun_p != 0, "Error getting func %s: %s\n", plugins[i].fun_info->func_name, dlerror());
            break;
        
        case FLOAT:
            plugins[i].func_p.floatfun_p = dlsym(plugins[i].lib_p, plugins[i].fun_info->func_name);
            AssertFatal(plugins[i].func_p.floatfun_p != 0, "Error getting float func %s: %s\n", plugins[i].fun_info->func_name, dlerror());

            break;
        
        default:
            AssertFatal(1 == 0, "Unknown return type in struct func_info in %s. Only int and float supported\n", direntP[i]->d_name);
            break;
        }
    
        

        libs_loaded++;
    }
    
    return libs_loaded;
}


int file_filter(const struct dirent *entry) {

    if ((entry->d_type == DT_REG) && (strstr(entry->d_name, ".so") != NULL))
        return 1;

    return 0;
}

void displayUsage(plugins_info_t *plugins, int num_of_plg) {

    printf("Available functions:\n");
    for (int i = 0; i < num_of_plg; i++) {
        printf("[%d] %s\n", i, plugins[i].fun_info->func_name);
    }
    printf("To use a function enter function name given above.\nPlease enter numbers separated by enter.\nTo exit program type: exit\n");

}

void exit_function(int plg_loaded, plugins_info_t *plugins, int *calc_exit) {
    
    *calc_exit = 1;

    // Need to call this in free function
    printf("Exiting..\n");
    for (int i = 0; i < plg_loaded; i++) {
        dlclose(plugins[i].lib_p);
    }
    free(plugins);

}

int main() {

    char **argv;
    char buf[12];
    char funcname[64];

    int plg_idx = -1;
    int plg_loaded = 0;

    int calc_exit = 0;
    struct dirent **direntP;

    int num_of_plg = scandir("./plugins", &direntP, file_filter, alphasort);
    AssertFatal(num_of_plg < 0, "Error scanning dir 'plugins'!%s", "\n");

    // array of libs
    plugins_info_t *plugins = (plugins_info_t *)malloc(num_of_plg*sizeof(plugins_info_t));
    AssertFatal((plugins != NULL), "Memmory allocation error for plugins %s", "\n");

    plg_loaded = plugin_load(plugins, direntP, num_of_plg);

    printf("%d/%d Plugins successfully loaded\n", plg_loaded, num_of_plg);

    // No more need in library names
    for (int i = 0; i < num_of_plg; i++) {
        // printf("freeing: %s\n", direntP[i]->d_name);
        free(direntP[i]);
    }
    free(direntP);

    displayUsage(plugins, num_of_plg);

    // User Interface here
    while (!calc_exit) {
        plg_idx = -1;

        printf("#:");

        fgets(funcname, 64, stdin);
        // to remove \n at the end
        funcname[strlen(funcname)-1] = '\0';

        if (strcmp(funcname, "help") == 0) {
            displayUsage(plugins, num_of_plg);
            continue;
        }

        // free memory and exit
        if (strcmp(funcname, "exit") == 0) {
            exit_function(plg_loaded, plugins, &calc_exit);
            continue;
        }

        // Naive function search
        for (int i = 0; i < plg_loaded; i++) {

            if (strcmp(plugins[i].fun_info->func_name, funcname) == 0) {
                plg_idx = i;
                break;
            }
        }

        if (plg_idx == -1) {
            printf("Function not found try: help\n");
            continue;
        }

        printf("This function requires %d arguments.\n", plugins[plg_idx].fun_info->number_of_args);

        argv = (char **)malloc(sizeof(plugins[plg_idx].fun_info->number_of_args));

        for (int i = 0; i < plugins[plg_idx].fun_info->number_of_args; i++) {
            argv[i] = (char *)malloc(12*sizeof(char));
            printf("#:");

            fgets(buf, 12, stdin);
            strcpy(argv[i], buf);
        }

        printf("Your input:\n");
        for (int i = 0; i < plugins[plg_idx].fun_info->number_of_args; i++) {
            printf("%s", argv[i]);
        }

        if (plugins[plg_idx].fun_info->return_type == INT) {
            printf("result of %s = %d\n", plugins[plg_idx].fun_info->func_name, plugins[plg_idx].func_p.intfun_p(plugins[plg_idx].fun_info->number_of_args, argv));
        } else if (plugins[plg_idx].fun_info->return_type == FLOAT) {
            printf("result of %s = %f\n", plugins[plg_idx].fun_info->func_name, plugins[plg_idx].func_p.floatfun_p(plugins[plg_idx].fun_info->number_of_args, argv));

        }

        for (int i = 0; i < plugins[plg_idx].fun_info->number_of_args; i++) {
            free(argv[i]);
        }
        free(argv);

    }


    return 0;
}