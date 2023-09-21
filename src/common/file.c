#include "file.h"

char *file_read(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory
    char *fileContent = (char *)malloc(fileSize + 1);
    if (!fileContent) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    // Read the file content
    size_t bytesRead = fread(fileContent, 1, fileSize, file);
    if (bytesRead != fileSize) {
        perror("Error reading file");
        free(fileContent);
        fclose(file);
        return NULL;
    }

    // Add null terminator to make it a valid string
    fileContent[fileSize] = '\0';

    fclose(file);
    return fileContent;
}
