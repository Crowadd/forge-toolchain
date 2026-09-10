#include <stdint.h>
#include <stdbool.h>

typedef enum Encoding {
	UTF_8, UTF_16LE, UTF_16BE
} Encoding;

typedef struct FileInfo {
	char *filePath;
     	uint8_t *fileData;
	uint8_t *fileEnd;
	uint8_t *current;
	Encoding encType;
} FileInfo;

int CopyFile(FileInfo *fileInfo, char *filePath);
void CreateFile(FileInfo fileInfo);


// put it somewhere else
typedef struct arguments {
	bool preProcessorFile;
	char **macros;
	int countMacros;
} arguments;
