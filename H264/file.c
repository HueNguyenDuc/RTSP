
#include "file.h"

unsigned int readFile(FILE* file, unsigned char* dataToRead, int* size) {
	int tSize = 0;
	if (file != NULL) {
	  	if(dataToRead == NULL)
	  		dataToRead = (unsigned char *)malloc(sizeof(unsigned char)*(*size));
	  	if(dataToRead == NULL)
	  		return 0;
	    unsigned int ch;
	    while ((ch = fgetc(file)) != EOF && tSize < (*size)) {
	    	*(dataToRead + tSize) = 0x00;
	    	*(dataToRead + tSize) = (unsigned char)(ch&0xff);
	        ++tSize;
	    }
	    if(tSize == 0) 
		{
			*size = 0;
			return 0;
		}
	    *size = tSize;
		return 1;
	}
	return 0;
};

unsigned int getFileSize(FILE* file)
{
	unsigned int curPointer = ftell(file);
	fseek(file, 0, SEEK_END);
	unsigned int size = ftell(file);
	fseek(file, curPointer, SEEK_SET);
	return size;
}