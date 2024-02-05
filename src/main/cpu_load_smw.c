#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

void cpu_load_SMW(cpu* myCpu)
{
    FILE *file;
    char *buffer;
    long file_size;

    // Open the file in binary mode
    if (fopen_s(&file, "smw.smc", "rb") != 0) {
        perror("Error opening file");
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    // Allocate memory for the byte array
    buffer = (char *)malloc(file_size);

    if (buffer == NULL) {
        perror("Memory allocation failed");
        fclose(file);
    }

    // Read the file into the buffer
    fread(buffer, 1, file_size, file);

    // Now 'buffer' contains the file data as a byte array
    for(int i = 0; i < file_size; i++)
        cpu_write_8bit(myCpu, 0x8000 + i, buffer[i]);

    // Close the file and free the allocated memory
    fclose(file);
    free(buffer);
}