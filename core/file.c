#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "file.h"
#include "wrappers.h"

int ReadFile(FileInfo *fileInfo, const char *filePath)
{

	if (!fileInfo)
		return 1;

	*fileInfo = (FileInfo){0};

	if (!filePath)
		return 1;

	FILE *file = xfopen(filePath, "rb");
	if (!file)
		return 1;

	if (xfseek(file, 0, SEEK_END)){
		xfclose(file);
		return 1;
	}

	long fileSize = xftell(file);
	if (fileSize == -1){
		xfclose(file);
		return 1;
	}


	uint8_t *data = xmalloc(sizeof(uint8_t) * (fileSize + 1));

	if (xfseek(file, 0, SEEK_SET)){
		free(data);
		xfclose(file);
		return 1;
	}

	if (xfread(data, 1, fileSize, file) < (size_t)fileSize){
		free(data);
		xfclose(file);
		return 1;
	}

	if (xfclose(file)){
		free(data);
		return 1;
	}
	char *path = xstrdup(filePath);

	fileInfo->fileData = data;
	fileInfo->filePath = path;
	fileInfo->fileEnd = fileInfo->fileData + fileSize;
	fileInfo->current = fileInfo->fileData;
	fileInfo->fileData[fileSize] = '\0';

	return 0;
}

int CreateNewFile(const FileInfo *fileInfo)
{
	if (!fileInfo || !fileInfo->filePath || !fileInfo->fileEnd || !fileInfo->fileData || fileInfo->fileEnd < fileInfo->fileData)
		return 1;

	FILE *file = xfopen(fileInfo->filePath, "wb");
	if (!file)
		return 1;

	size_t fileSize = fileInfo->fileEnd - fileInfo->fileData;
	if (xfwrite(fileInfo->fileData, 1, fileSize, file) != fileSize){
		xfclose(file);
		return 1;
	}

	if (xfclose(file))
		return 1;
	return 0;

}

void FreeFileInfo(FileInfo *fileInfo)
{
	if (!fileInfo)
		return;
	free(fileInfo->fileData);
	free(fileInfo->filePath);
	*fileInfo = (FileInfo) {0};
	return;
}
