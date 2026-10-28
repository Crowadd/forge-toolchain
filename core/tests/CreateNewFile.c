#include <stdio.h>
#include "core.h"

int main(int argc, char *argv[])
{
	if (argc < 2){
		fprintf(stderr, "Input a file!\n");
		return 1;
	}

	FileInfo file = {0};
	if(ReadFile(&file, argv[1]))
		return 1;

	file.filePath = argv[2];
	CreateNewFile(&file);
	FreeFileInfo(&file);
	return 0;
}
