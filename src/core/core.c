#include "core.h"


const char* CENGINE_CORE_ReadFile(const char* file_path) 
{
    FILE *file = fopen(file_path, "r");
    if(file == NULL) {
        perror("Failed to read file.");
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char* source = (char*)malloc((file_size + 1) * sizeof(char));
    if(source == NULL) {
        perror("Error allocating memory.");
        fclose(file);
    }

    fread(source, 1, file_size, file);
    source[file_size] = '\0';

    fclose(file);
    return source;
}
