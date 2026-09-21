#include <stdio.h>
#include <stdlib.h>
#include "core.h"

FILE *xfopen(char *path, char *mode)
{
	FILE *file = fopen(path, mode);
	if (!file)
		perror(PROGRAM_NAME);
		
	return file;
}

int xfseek(FILE *stream, long offset, int whence)
{
	int ret = fseek(stream, offset, whence);
	if (ret == -1){
		perror(PROGRAM_NAME);
		exit(EXIT_FAILURE);
	}
	return ret;

}

long xftell(FILE *stream)
{
	long ret = ftell(stream);
	if (ret == -1){
		perror(PROGRAM_NAME);
		exit(EXIT_FAILURE);
	}
	return ret;
}

void *xmalloc(size_t size)
{
	void *ret = malloc(size);
	if (!ret){
		perror(PROGRAM_NAME);
		exit(EXIT_FAILURE);
	}
	return ret;
}

size_t xfread(void *ptr, size_t size, size_t n, FILE *stream)
{
	return fread(ptr, size, n, stream);
}

size_t xfwrite(void *ptr, size_t size, size_t n, FILE *stream)
{
	size_t ret = fwrite(ptr, size, n, stream);
	if (!ret){
		perror(PROGRAM_NAME);
		exit(EXIT_FAILURE);
	}
	return ret;
}


int xfclose(FILE *stream)
{
	int ret = fclose(stream);
	if (ret){
		perror(PROGRAM_NAME);
		exit(EXIT_FAILURE);
	}
	return ret;
}

void *xrealloc(void *p, size_t size)
{
	void *ret = realloc(p, size);
	if (!ret){
		perror(PROGRAM_NAME);
		exit(EXIT_FAILURE);
	}
	return ret;
}
