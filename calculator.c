#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <assert.h>
#include <dirent.h>
#include <string.h>
#include <malloc.h>

#define AssertFatal(cond, fmt, ...) do { if (!cond) { printf(fmt, __VA_ARGS__); assert(cond); } } while(0)

typedef struct func_info {
    char *func_name;
    char *return_type;
    unsigned char number_of_args;
} func_info_t;

typedef struct plugins_info {
    // pointer to global struct func_info in lib
    func_info_t *fun_info;
    // pointer to handle libs
    void *lib_p;
    // function pointer to function in lib
    int (*fun_p)();
} plugins_info_t;

int plugin_load(plugins_info_t **plugins, struct dirent **direntP, int num_of_pl) {

    int libs_loaded = 0;

    for (int i = 0; i < num_of_pl; i++) {

        printf("[%d/%d] Loading library %s\n", i+1, num_of_pl, direntP[i]->d_name);

        plugins[i]->lib_p = dlopen(direntP[i]->d_name, RTLD_LAZY);
        AssertFatal(plugins[i]->lib_p != 0, "Error loading library %s: %s\n", direntP[i]->d_name, dlerror());

        plugins[i]->fun_info = (func_info_t *)dlsym(plugins[i]->lib_p, "fun_info");
        AssertFatal(plugins[i]->fun_info != 0, "Error getting func_info in %s: %s\n", direntP[i]->d_name, dlerror());

        plugins[i]->fun_p = dlsym(plugins[i]->lib_p, plugins[i]->fun_info->func_name);
        AssertFatal(plugins[i]->fun_p != 0, "Error getting func %s: %s\n", plugins[i]->fun_info->func_name, dlerror());

        libs_loaded++;
    }
    
    return libs_loaded;
}


int file_filter(const struct dirent *entry) {

    if ((entry->d_type == DT_REG) && (strstr(entry->d_name, ".so") != NULL))
        return 1;

    return 0;
}

void displayUsage() {

}

int main() {

    int plg_loaded = 0;
    int calc_exit = 0;
    struct dirent **direntP;

    int num_of_plg = scandir("./", &direntP, file_filter, alphasort);
    AssertFatal(num_of_plg < 0, "Error scanning dir 'plugins'!%s", "\n");

    // array of libs
    plugins_info_t *plugins = (plugins_info_t *)malloc(3*sizeof(plugins_info_t));

    plg_loaded = plugin_load(&plugins, direntP, num_of_plg);

    printf("Plugins successfully loaded %d/%d\n", plg_loaded, num_of_plg);

    // No more need in library names
    for (int i = 0; i < num_of_plg; i++) {
        // printf("freeing: %s\n", direntP[i]->d_name);
        free(direntP[i]);
    }

    displayUsage();

    // User Interface here
    while (!calc_exit) {
        printf("#:");


        printf("+: %d\n", (int)plugins[0].fun_p(1, 2));
        

        calc_exit = 1;
    }


    // Need to call this in free function
    printf("Exiting..\n");
    for (int i = 0; i < plg_loaded; i++) {
        dlclose(plugins[i].lib_p);
    }
    free(plugins);

    return 0;
}