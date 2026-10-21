#include <stdio.h>
#include <string.h>

#include "core.h"

void usage()
{
	puts("\
Usage: forge-cc [OPTION...] [FILE...]\n\
	-h --help                 Display help information\n\
	-v --version              Current version of forge-cc\n\
	-o --output FILE_NAME     Output file name\
");
}

void version()
{
	printf("forge-cc ");
	puts(VERSION);
}

int main(int argc, char *argv[])
{

	struct flags {
		char *output;

		char **macros;
		int macroCount;

		char **undefMacros;
		int undefMacroCount;

		char **directories;
		int dirCount;

		bool preprocess;
		bool assemble;
		bool compile;
		
	} flags = {0};

	struct CliOption options[] = {
		{'v', "version", 0},
		{'h', "help", 0},
		{'o', "output", 1},
		{'E', "preprocess", 0},
		{'S', "assemble", 0},
		{'c', "compile", 0},
		{'D', "define-macro", 1},
		{'U', "undefine-macro", 1},
		{'I', "include-directory", 1},
		
		{0, NULL, 0}
	};

	struct CliReturn ret = {0};
	do {
		ret = CliParse(options, &ret, argc, argv);
		switch(ret.option){
		case 'h': usage(); return 0;
		case 'v': version(); return 0;
		case 'o': flags.output = ret.argument; break;
		case 'E': flags.preprocess = true; break;
		case 'S': flags.assemble = true; break;
		case 'c': flags.compile = true; break;
		case 'D':
			  flags.macroCount++;
			  flags.macros = xrealloc(flags.macros, flags.macroCount*sizeof(char *));
			  flags.macros[flags.macroCount - 1] = ret.argument;
			  break;
		case 'U':
			  flags.undefMacroCount++;
			  flags.undefMacros = xrealloc(flags.undefMacros, flags.undefMacroCount*sizeof(char *));
			  flags.macros[flags.macroCount - 1] = ret.argument;
			  break;
		case 'I':
			  flags.dirCount++;
			  flags.directories = xrealloc(flags.directories, flags.dirCount*sizeof(char *));
			  flags.directories[flags.dirCount - 1] = ret.argument;
			  break;

		case -1: usage(); return 1;

		}
	} while (ret.option);

	// E,S,c,o,I,D,U,include,std=,Wall,Wextra,Werror,w,pedantic,pedantic-errors,g,O0,O1,O2,O3
	
	if (flags.preprocess){
		printf("Preprocess only! ");
		printf("output: %s\n", (flags.output) ? flags.output : "stdout");
	}

	if (flags.macroCount)
		for (int i = 0; i < flags.macroCount; i++)
			printf("%d: %s\n", i, flags.macros[i]);

	if (flags.undefMacroCount)
		for (int i = 0; i < flags.undefMacroCount; i++)
			printf("%d: %s\n", i, flags.undefMacros[i]);

	if (flags.dirCount)
		for (int i = 0; i < flags.dirCount; i++)
			printf("%d: %s\n", i, flags.directories[i]);

	if (flags.output)
		printf("output: %s\n", flags.output);


	return 0;
}
