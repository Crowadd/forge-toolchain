#include <stdio.h>
#include <stdbool.h>

#include "forge-cc.h"

#define INITIAL_CAPACITY 256

void Cpp(char *filePath, arguments args);

typedef enum CppTokenType {
	CPP_TOKEN_IDENTIFIER,
	CPP_TOKEN_PUNCTUATOR,
	CPP_TOKEN_NEW_LINE,
	CPP_TOKEN_STRING,
	CPP_TOKEN_CHAR,
	CPP_TOKEN_OTHER,
} CppTokenType;

typedef struct CppToken {
	CppTokenType type;	
	char *str;
	size_t length;
} CppToken;

typedef struct CppTokenStorage {
	CppToken *tokens;
	int tokensCount;
	size_t capacity;
	unsigned int line;
} CppTokenStorage;

