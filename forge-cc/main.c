#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "core.h"


void usage()
{
	puts("\
Usage: forge-cc [OPTION...] [FILE...]\n\
	-h --help                 Display help information\n\
	-v --version              Current version of forge-cc\n\
	-o --output FILE_NAME     Output file name\n\
	-E --preprocess           Preprocess only\n\
	-S --assembler            Assemble only\n\
	-c --compile              compile without linking\n\
	-D --define-macro NAME    Define macro\n\
	-U --undefine-macro NAME  Undefine macro\n\
	-I --include-directory    Include directory for preprocess\n\
	-i --include FILE_NAME	  Include like #include\n\
	-s --std NAME             C standart to be used\n\
	-W --warning NAME         Warning type to be used\n\
	-g --debug                Aditional information debugging\n\
	-O --optimize NAME        Optimization level\n\
");
}

void version()
{
	printf("%s: %s\n", PROGRAM_NAME, VERSION);
}

int main(int argc, char *argv[])
{

	struct flags {
		char *output;
		char *std;
		char *optimization;

		char **macros;
		unsigned int macroCount;

		char **undefMacros;
		unsigned int undefMacroCount;

		char **directories;
		unsigned int dirCount;

		char **includes;
		unsigned int includeCount;

		char **warnings;
		unsigned int warnCount;

		bool preprocess;
		bool assemble;
		bool compile;
		bool debug;
		
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
		{'i', "include", 1},
		{'s', "std", 1},
		{'W', "warning", 1},
		{'g', "debug", 0},
		{'O', "optimize", 1},
		{0, NULL, 0}
	};

	struct CliReturn ret = {0};
	do {
		ret = CliParse(options, &ret, argc, argv);
		switch(ret.option){
		case 'h': usage(); return 0;
		case 'v': version(); return 0;
		case 'o': flags.output = ret.argument; break;
		case 's': flags.std = ret.argument; break;
		case 'E': flags.preprocess = true; break;
		case 'S': flags.assemble = true; break;
		case 'c': flags.compile = true; break;
		case 'g': flags.debug = true; break;

		case 'D':
			  flags.macroCount++;
			  flags.macros = xrealloc(flags.macros, flags.macroCount*sizeof(char *));
			  flags.macros[flags.macroCount - 1] = ret.argument;
			  break;
		case 'U':
			  flags.undefMacroCount++;
			  flags.undefMacros = xrealloc(flags.undefMacros, flags.undefMacroCount*sizeof(char *));
			  flags.undefMacros[flags.undefMacroCount - 1] = ret.argument;
			  break;
		case 'I':
			  flags.dirCount++;
			  flags.directories = xrealloc(flags.directories, flags.dirCount*sizeof(char *));
			  flags.directories[flags.dirCount - 1] = ret.argument;
			  break;
		case 'i':
			  flags.includeCount++;
			  flags.includes = xrealloc(flags.includes, flags.includeCount*sizeof(char *));
			  flags.includes[flags.includeCount - 1] = ret.argument;
			  break;
		case 'W':
			  flags.warnCount++;
			  flags.warnings = xrealloc(flags.warnings, flags.warnCount*sizeof(char *));
			  flags.warnings[flags.warnCount - 1] = ret.argument;
			  break;

		case -1: usage(); return 1;

		}
	} while (ret.option);

	// E,S,c,o,I,D,U,include,std=,Wall,Wextra,Werror,w,pedantic,pedantic-errors,g,O0,O1,O2,O3
	
	if (ret.argc > argc || argc < 2){
		fprintf(stderr, "%s: No input file\n", PROGRAM_NAME);
		return 1;
	}
	
	FileInfo file;
	if (!CopyFile(&file, argv[ret.argc - 1]))
		printf("%s\n", file.fileData);

		
	
	

	return 0;
}
