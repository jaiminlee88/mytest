#include <stdio.h>
#include <dlfcn.h>

int main(int argc, char* argv[]) {
    void* handle;
    using FUNC = double (*)(int);
    FUNC func;
    char* error;
    printf("argv[] %s\n", argv[1]);

    handle = dlopen(argv[1],RTLD_NOW);
    if(handle == NULL) {
        printf("Open library %s error: %s\n", argv[1], dlerror());
        return -1;
    }

    func = (FUNC)dlsym(handle,argv[2]);
    printf(" %d\n", 11111);
    if( (error = dlerror()) != NULL ) {
        printf("Symbol %s not found: %s\n",argv[2], error);
        goto exit_runso;
    }

    printf( "%f\n", func(1000) );

    exit_runso:
    dlclose(handle);
}