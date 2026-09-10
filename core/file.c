#include <stdlib.h>
#include <stdio.h>
#include "core.h"
#include "file.h"

int CopyFile(FileInfo *fileInfo, char *filePath)
{

	FILE *file = xfopen(filePath, "rb");
	if (!file)
		return 1;

	xfseek(file, 0, SEEK_END);
	size_t fileSize = xftell(file);
	fileInfo->fileData = xmalloc(fileSize + 1);
	xfseek(file, 0, SEEK_SET);

	if (xfread(fileInfo->fileData, 1, fileSize, file) < fileSize){
		perror("fread");
		free(fileInfo->fileData);
		exit(EXIT_FAILURE);
	}
	fileInfo->fileEnd = fileInfo->fileData + fileSize;
	fileInfo->current = fileInfo->fileData;
	fileInfo->fileData[fileSize] = '\0';
	fileInfo->filePath = filePath;

	xfclose(file);
	return 0;
}

void CreateFile(FileInfo fileInfo)
{
	FILE *file = xfopen(fileInfo.filePath, "wb");
	if (!file)
		exit(EXIT_FAILURE);

	size_t fileSize = fileInfo.fileEnd - fileInfo.fileData;
	xfwrite(fileInfo.fileData, 1, fileSize, file);

	xfclose(file);
	return;

}
