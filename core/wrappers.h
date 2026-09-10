#ifndef WRAPPERS_H
#define WRAPPERS_H

#include <stdio.h>
#include <stdlib.h>
FILE *xfopen(char *path, char *mode);
int xfseek(FILE *stream, long offset, int whence);
long xftell(FILE *stream);
void *xmalloc(size_t size);
size_t xfread(void *ptr, size_t size, size_t n, FILE *stream);
size_t xfwrite(void *ptr, size_t size, size_t n, FILE *stream);
int xfclose(FILE *stream);

#endif
