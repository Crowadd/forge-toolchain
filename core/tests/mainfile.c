#include <stdio.h>

#define PROGRAM_NAME "filetest" 
#include "../file.h"

int main(int argc, char **argv)
{
	if (argc < 2){
		fprintf(stderr, "Input a file!\n");
		return 1;
	}

	FileInfo file = {0};
	if(CopyFile(&file, argv[1]))
		return 1;
	printf("%s\n", file.fileData);
	return 0;
}
