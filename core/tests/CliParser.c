#include "CliParser.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	struct CliOption cli[] = {
		{'h', "help", NO_ARGUMENT},
		{'v', "version", NO_ARGUMENT},
		{'o', "output", ARGUMENT_REQUIRED},
		{'D', NULL, ARGUMENT_REQUIRED},
		{0, NULL, 0}
	};

	struct CliReturn ret = {0};
	do {
		ret = CliParse(cli, &ret, argc, argv);
		switch(ret.option){
		case 'h':
			printf("HELP\n");
			break;
		case 'v':
			printf("Version 0.0.1\n");
			break;
		case 'o':
			printf("output: %s\n", ret.argument);
			break;
		case 'D':
			printf("#define %s\n", ret.argument);
			break;
		case -1:
			printf("Unknow option!\n");
			break;
		}
	} while (ret.option);
	printf("program args: %s\n", argv[ret.argc-1]);
}
