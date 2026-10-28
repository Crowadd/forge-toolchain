#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wrappers.h"

FILE *xfopen(const char *path, const char *mode)
{
	FILE *file = fopen(path, mode);
	if (!file)
		perror(PROGRAM_NAME);
		
	return file;
}

int xfseek(FILE *stream, long offset, int whence)
{
	int ret = fseek(stream, offset, whence);
	if (ret == -1)
		perror(PROGRAM_NAME);
	return ret;

}

long xftell(FILE *stream)
{
	long ret = ftell(stream);
	if (ret == -1)
		perror(PROGRAM_NAME);
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
	size_t ret = fread(ptr, size, n, stream);
	if (ferror(stream))
		perror(PROGRAM_NAME);
	return ret;
}

size_t xfwrite(const void *ptr, size_t size, size_t n, FILE *stream)
{
	size_t ret = fwrite(ptr, size, n, stream);
	if (ret != n)
		perror(PROGRAM_NAME);
	return ret;
}


int xfclose(FILE *stream)
{
	int ret = fclose(stream);
	if (ret)
		perror(PROGRAM_NAME);
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

char *xstrdup(const char *str)
{
	if (!str)
		return NULL;
	size_t length = strlen(str);
	char *tmp = xmalloc(length * sizeof(char) + 1);
	return memcpy(tmp, str, length + 1);
}
