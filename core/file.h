#ifndef FILE_H
#define FILE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct FileInfo {
	char *filePath;
     	uint8_t *fileData;
	uint8_t *fileEnd;
	uint8_t *current;
} FileInfo;

int ReadFile(FileInfo *fileInfo, const char *filePath);
int CreateNewFile(const FileInfo *fileInfo);
void FreeFileInfo(FileInfo *fileInfo);

#endif
