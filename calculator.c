#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <assert.h>

#define AssertFatal(cond, fmt, ...) do { if (!cond) printf(fmt, __VA_ARGS__); assert(cond); } while(0)


void *dyn_load(char *libname, char *dsym) {

    void *handle = dlopen(libname, RTLD_LAZY);
    AssertFatal(handle != 0, "%s\n", dlerror());

    int (*add_ptr)(int, int);

    add_ptr = dlsym(handle, dsym);
    AssertFatal(add_ptr != 0, "%s\n", dlerror());


     = dlsym(handle, dsym);
    AssertFatal(add_ptr != 0, "%s\n", dlerror());

    printf("1 + 1 = %d\n", add_ptr(1, 1));

    return handle;
}

int main() {

    void *handle = dyn_load("libmath.so.1", "add");



    // printf("1 + 1 = %d\n", add_ptr(1, 1));



    dlclose(handle);

    return 0;
}