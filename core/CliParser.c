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

	if (ret->argc == argc && !ret->current){
		*ret = (CliReturn){.argc = ret->argc};
		return *ret;
	}

	if (ret->argc < argc && !ret->current)
		ret->argc++;

	ret->current = (!ret->current) ? argv[ret->argc - 1] : ret->current;

	if (argv[ret->argc-1][0] != '-' || strlen(argv[ret->argc-1]) < 2){
		*ret = (CliReturn){.argc = ret->argc};
		return *ret;
	}

	bool isInvalid = true;
	bool isLongOption = false;

	if (argv[ret->argc-1][1] == '-'){
		if (argv[ret->argc-1][2] == '\0'){
			*ret = (CliReturn){.argc = ret->argc};
			return *ret;
		}

		isLongOption = true;
		ret->current++;
	}

	ret->current++;
	
	while (cli->option || cli->longOption){
		if (isLongOption){
			if (!cli->longOption){
				cli++;
				continue;
			}

			size_t length;
			const char *equal = strchr(ret->current, '=');
			if (cli->arg != NO_ARGUMENT && equal)
				length = equal - ret->current;
			else
				length = strlen(ret->current);


			size_t longOptionSize = strlen(cli->longOption);
			if (longOptionSize != length){
				cli++;
				continue;
			}
			if (!memcmp(ret->current, cli->longOption, longOptionSize)){
				ret->retOption = cli->retOption;
				isInvalid = false;
				ret->current += longOptionSize - 1;
				break;
			}
				
		} else {
			if (*ret->current == cli->option){
				ret->retOption = cli->retOption;
				isInvalid = false;
				break;
			}
		}
		cli++;
	}

	if (isInvalid){
		*ret = (CliReturn){.retOption = -1, .argc = ret->argc};
		return *ret;
	}

	if (cli->arg == NO_ARGUMENT){
		if (*(ret->current + 1) == '\0' || isLongOption){
			ret->current = NULL;
			return *ret;
		}
		return *ret;
	}

	if (*(ret->current + 1) == '\0'){
		if (ret->argc < argc){
			ret->argc++;
			ret->argument = argv[ret->argc-1];
		} else {
			ret->retOption = ':';
			ret->argument = NULL;			
		}
	} else {
		if (isLongOption){
			if (ret->current[1] == '=' && ret->current[2] != '\0'){
				ret->argument = ret->current + 2;
			} else{
				ret->retOption = ':';
				ret->argument = NULL;
			}
		} else {
			ret->argument = ret->current + 1;
		}
	}

	ret->current = NULL;
	return *ret;

}
