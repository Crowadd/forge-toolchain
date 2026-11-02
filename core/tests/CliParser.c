#include "CliParser.h"
#include <stdio.h>

int main(int argc, const char *argv[])
{
	struct CliOption cli[] = {
		{0, "help", 'h', NO_ARGUMENT},
		{'v', "version", 'v', NO_ARGUMENT},
		{'a', "verdade", 'a', NO_ARGUMENT},
		{'o', "output", 'o', ARGUMENT_REQUIRED},
		{'D', NULL, 'D', ARGUMENT_REQUIRED},
		{0, NULL, 0, 0}
	};

	struct CliReturn ret = {0};
	do {
		ret = CliParse(cli, &ret, argc, argv);
		switch(ret.retOption){
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
		case 'a':
			printf("TA FUNCIONANDO\n");
			break;
		case -1:
			printf("Unknow option!\n");
			break;
		case ':':
			printf("Missing argument!\n");
			break;
		}
	} while (ret.retOption);
	printf("program args: %s\n", argv[ret.argc-1]);
}
