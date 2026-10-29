#include "CliParser.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

CliReturn CliParse(CliOption *cli, CliReturn *ret, int argc, const char *argv[])
{
	if (!cli || !ret || !argv || argc < 2)	
		return (CliReturn){0};

	if (!ret->argc){
		*ret = (CliReturn){0};
		ret->argc = 1;
	}

	if (ret->argc == argc && !ret->current)
		return (CliReturn){.argc = ret->argc};

	if (ret->argc < argc && !ret->current)
		ret->argc++;

	ret->current = (!ret->current) ? argv[ret->argc - 1] : ret->current;

	if (argv[ret->argc-1][0] != '-' || strlen(argv[ret->argc-1]) < 2)
		return (CliReturn){.argc = ret->argc};

	bool isInvalid = true;
	bool isLongOption = false;

	if (argv[ret->argc-1][1] == '-'){
		if (argv[ret->argc-1][2] == '\0')
			return (CliReturn){.argc = ret->argc};

		isLongOption = true;
	}

	ret->current++;
	
	while (cli->option){
		if (isLongOption && cli->longOption){
			ret->current++;
			size_t longOptionSize = strlen(cli->longOption);
			if (longOptionSize > strlen(ret->current)){
				cli++;
				continue;
			}
			if (!memcmp(ret->current, cli->longOption, longOptionSize)){
				ret->option = cli->option;
				isInvalid = false;
				ret->current += longOptionSize;
				break;
			}
				
		} else {
			if (*ret->current == cli->option){
				ret->option = cli->option;
				isInvalid = false;
				break;
			}
		}
		cli++;
	}

	if (isInvalid)
		return (CliReturn){.option = -1, .argc = ret->argc};

	if (cli->arg == NO_ARGUMENT){
		if (*(ret->current + 1) == '\0' || isLongOption){
			ret->current = NULL;
			return *ret;
		}
		return *ret;
	}

	if (*(ret->current + 1) == '\0' || (isLongOption && *(ret->current) != '-')){
		if (ret->argc < argc){
			ret->argc++;
			ret->argument = argv[ret->argc-1];
		} else {
			ret->argument = NULL;			
		}
	} else {
		ret->argument = ret->current + 1;
	}

	ret->current = NULL;
	return *ret;

}
