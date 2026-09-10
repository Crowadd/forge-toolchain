#include <getopt.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "cpp.h"

void usage()
{

	printf("help usage:\n\
forge-cc [options] [SOURCE_CODE]\n\
	--preprocess -E : save preprocessed source code file\n\
	--define-macro -D : define macro\n\
	\n\
	--help -h : help info\n\
	--version -v : version\n");
	return;
}

void version()
{
	printf("forge-cc ");
	puts(VERSION);
}

int main(int argc, char **argv)
{

	arguments args = {0};

	struct option options[] = {
		{"help", no_argument, NULL, 'h'},
		{"version", no_argument, NULL, 'v'},
		{"preprocess", no_argument, NULL, 'E'},
		{"define-macro", required_argument, NULL, 'D'},
		{NULL, 0, NULL, 0}
	};
	int c;
	while ((c = getopt_long(argc, argv, "hvED:", options, NULL)) != -1){
		switch (c){
		case 'h': usage(); return 0;
		case 'v': version(); return 0;
		case '?': usage(); return 1;
		case 'E': args.preProcessorFile = true; break;
		case 'D':
			args.countMacros++;
			args.macros = realloc(args.macros, args.countMacros * sizeof(char *));
			args.macros[args.countMacros-1] = optarg;
			break;
		}
	}

	char *filePath = argv[optind];
	if (!filePath){
		fprintf(stderr, "file missing\n");
		exit(EXIT_FAILURE);
	}
	Cpp(filePath, args);	


}
