#include "errors.h"
void Warning(FileInfo *pp, int line, char *str)
{
	printf("WARNING:%s:%d - %s\n", pp->filePath, line, str);
}

