#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "cpp.h"
#include "errors.h"

static void NewToken(CppTokenStorage *storage, CppTokenType type, char *str, size_t length)
{
	if (!storage->tokens){
		storage->capacity = INITIAL_CAPACITY;
		storage->tokensCount = 0;
		storage->line = 1;
		storage->tokens = malloc(storage->capacity * sizeof(CppToken));

		if (!storage->tokens){
			perror("malloc");
			exit(EXIT_FAILURE);
		}
	}

	if (type == CPP_TOKEN_NEW_LINE)
		storage->line++;
	
	CppToken *token = &storage->tokens[storage->tokensCount];
	token->type = type;
	token->str = str;
	token->length = length;
	storage->tokensCount++;

	if (storage->tokensCount > storage->capacity){
		storage->capacity *= 2;
		storage->tokens = realloc(storage->tokens, storage->capacity*sizeof(CppToken));

		if (!storage->tokens){
			perror("realloc");
			exit(EXIT_FAILURE);
		}
		//ExecTokens(storage);
	}

#ifdef DEBUGGING
	printf("token %d: ", storage->tokensCount);
	switch (token->type){
	case CPP_TOKEN_STRING: printf("CPP_TOKEN_STRING"); break;
	case CPP_TOKEN_NEW_LINE: printf("CPP_TOKEN_NEW_LINE"); break;
	}
	printf("\n");
	if (token->str)
		printf("%.*s\n", token->length ,token->str);
	
#endif

}

static void IgnoreComment(FileInfo *pp, CppTokenStorage *storage)
{
	unsigned int warningLine;
	if (*pp->current != '/')
		return;

	pp->current++;
	switch (*(pp->current)){
	case '/':
#ifdef DEBUGGING
		printf("// comment\n");
#endif
		pp->current++;
		while (*pp->current != '\n'){
			if (*pp->current == '\\' && *(pp->current+1) == '\n'){
				pp->current += 2;
				storage->line++;
			}
			pp->current++;
		}
		break;

	case '*':
#ifdef DEBUGGING
		printf("/* comment\n");
#endif
		warningLine = storage->line;
		pp->current++;
		while (*pp->current != '*' || *(pp->current+1) != '/'){

			if (*pp->current == '\\' && *(pp->current+1) == '\n'){
				pp->current += 2;
				storage->line++;
				continue;
			}
			if (*pp->current == '\n'){
				NewToken(storage, CPP_TOKEN_NEW_LINE, NULL, 1);
				pp->current++;
				continue;
			}
			if (pp->current > pp->fileEnd){
				Warning(pp, warningLine, "missing terminating comment sequence \"*/\" characters");
				return;
			}
			pp->current++;
		}
		pp->current += 2;
		break;
	}
	return;
}

static void TokenizeString(FileInfo *pp, CppTokenStorage *storage)
{
	unsigned int warningLine = storage->line + 1;
	int length = 1;

	if (*pp->current != '\"')
		return;

	length++;
	pp->current++;
	while (*pp->current != '\"'){

		if (*pp->current == '\\' && *(pp->current+1) == '\n'){
			// Back to the last character to don't save '\\'
			pp->current--;
			length--;

			NewToken(storage, CPP_TOKEN_STRING, pp->current-length+1, length);
			storage->line++;
			length = 1;
			// Go to the character after '\n'
			pp->current += 3;
			continue;
		}
		if (*pp->current == '\n'){
			// Back to the last character to don't save '\n'
			pp->current--;
			length--;

			Warning(pp, warningLine, "missing terminating \" character");
			NewToken(storage, CPP_TOKEN_STRING, pp->current-length+1, length);
			return;
		}
		length++;
		pp->current++;
	}

	NewToken(storage, CPP_TOKEN_STRING, pp->current-length+1, length);
	pp->current++;
	
	return;
}

static void TokenizeChar(char **current, CppTokenStorage *storage){}
static void TokenizePunctuator(char **current, CppTokenStorage *storage){}
static void TokenizeIdentifier(char **current, CppTokenStorage *storage){}
static void TokenizeOther(char **current, CppTokenStorage *storage){}

static void PreProcess(char *filePath)
{
	FileInfo pp = {0};
	CopyFile(&pp, filePath);
	CppTokenStorage storage = {0};
	// InitTokenStorage(&storage);


	while (pp.current < pp.fileEnd){
		while (isspace(*pp.current) && *pp.current != '\n')	
			pp.current++;

		if (*pp.current == '\\' && *(pp.current+1) == '\n')
			pp.current += 2;

		if (*pp.current == '\n'){
			NewToken(&storage, CPP_TOKEN_NEW_LINE, NULL, 1);
			pp.current++;
		}

		IgnoreComment(&pp, &storage);
		TokenizeString(&pp, &storage);
		//TokenizeChar(&pp, &storage);
		//TokenizePunctuator(&pp, &storage);
		//TokenizeIdentifier(&pp, &storage);
		//TokenizeOther(&pp, &storage);
	}

	free(pp.fileCopy);
}

void Cpp(char *filePath, arguments args)
{

	PreProcess(filePath);

	
	
}
