#include "../include/file_manager.h"

int open_file(FILE** file, char const* file_name, char const* mode){
    *file = fopen(file_name, mode);
    return *file != NULL;
}

int close_file(FILE* file){
    return !fclose(file);
}

