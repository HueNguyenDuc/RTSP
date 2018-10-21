#ifndef _FILE_H
#define _FILE_H


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAXFILENAME 256

unsigned int readFile(FILE* file, unsigned char* dataToRead, int* size);
unsigned int getFileSize(FILE* file);

#endif // !_FILE_H