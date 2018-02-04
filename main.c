#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Check.h"

// TODO: handle NOCPLC tag

int main(int argc, char **argv)
{
    printf("CoDeSys Project Line Counter\n");
    printf("Version 1.0\n");
    printf("(C) 2018 Martial Demolins\n");
    printf("License: GPL v3\n");

    // Check the command line parameters
    if (argc != 2) {
        printf("Usage: cplc <CoDeSys project file>\n");
        return EXIT_FAILURE;
    }

    // Open the file
    FILE* file = fopen(argv[1], "rb");
    if (file == NULL) {
        fprintf(stderr, "File %s not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Read the file size
    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);

    // Create a buffer to put file content
    char* buffer = malloc(size);
    if (buffer == NULL) {
        fprintf(stderr, "Couldn't allocate memory to read the file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Copy file content in the buffer
    rewind(file);
    if (fread(buffer, size, 1, file) != 1) {
        fprintf(stderr, "Error while reading file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Close the file
    fclose(file);

    // Check the magic of the .pro file
    if (memcmp(buffer, "CoDeSys", 7) != 0) {
        fprintf(stderr, "The file %s is not a valid CoDeSys file\n", argv[1]);
        return EXIT_FAILURE;
    }

    // ACK message
    printf("File %s found and successfully read\n", argv[1]);
    printf("Size: %I64u bytes\n", size);

    unsigned int linecount = 0;
    for (unsigned int i = 0; i < size - 1; i++) {
        if ((buffer[i] == 0x0D) && (buffer[i+1] == 0x0A)) {
            linecount++;
        }
    }

    printf("Lines found: %u\n", linecount);

    // Finally, free the buffer
    free(buffer);
}
