#include "CliParser.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static char *NextArgv(struct CliReturn *ret, int argc, char *argv[])
{
	ret->argc++;
	if (ret->argc <= argc)
		ret->current = argv[ret->argc-1] + 1;
	return (ret->argc <= argc) ? argv[ret->argc-1] : NULL;
}


static struct CliReturn ParseLongOption(struct CliOption cli[], struct CliReturn *ret, int argc, char *argv[])
{

	// skip -
	ret->current++;

	bool invalidOption = true;
	int length, lengthCurrent;
	while (cli->option){
		length = strlen(cli->longOption);
		lengthCurrent = strlen(ret->current);
		if (!memcmp(ret->current, cli->longOption, (lengthCurrent < length) ? lengthCurrent : length)){
			ret->option = cli->option;
			invalidOption = false;
			break;
		}
		cli++;
	}

	if (invalidOption){
		ret->option = -1;
		ret->argument = NULL;
		NextArgv(ret, argc, argv);
		return *ret;
	}
	
	if(cli->arg == ARGUMENT_REQUIRED){
		ret->current += length - 1;
		if (*(ret->current + 1) == '=' && *(ret->current + 2) != '\0'){
			ret->current += 2;
			ret->argument = ret->current;
		}
		else if (*(ret->current + 1) == '\0'){
			ret->argument = NextArgv(ret, argc, argv);
		}
		NextArgv(ret, argc, argv);
		return *ret;
	} 

	NextArgv(ret, argc, argv);
	return *ret;
}

static struct CliReturn ParseOption(struct CliOption cli[], struct CliReturn *ret, int argc, char *argv[])
{
	

	bool invalidOption = true;
	while (cli->option){
		if (*ret->current == cli->option){
			ret->option = cli->option;
			invalidOption = false;
			break;
		}
		cli++;
	}

	if (invalidOption){
		ret->option = -1;
		ret->argument = NULL;
		if (*(ret->current + 1) != '\0')
			ret->current++;
		else
			NextArgv(ret, argc, argv);
		return *ret;
	}
	
	if(cli->arg == ARGUMENT_REQUIRED){
		if (*(ret->current + 1) != '\0'){
			ret->current++;
			ret->argument = ret->current;
		}
		else{
			NextArgv(ret, argc, argv);
			ret->argument = (ret->argc <= argc) ? argv[ret->argc-1] : NULL; 
		}
		NextArgv(ret, argc, argv);
		return *ret;
	} 

	if (*(ret->current + 1) != '\0')
		ret->current++;
	else
		NextArgv(ret, argc, argv);

	return *ret;
}

static struct CliReturn CleanReturn(struct CliReturn *ret)
{
	ret->option = 0;
	ret->argument = NULL;
	ret->current = NULL;
	return *ret;
}


struct CliReturn CliParse(struct CliOption *cli, struct CliReturn *ret, int argc, char *argv[])
{

	// Initialize CliReturn
	if (!ret->argc){
		if (argc == 1){
			ret->argc = 1;
			return CleanReturn(ret);
		}
		ret->argc = 2;
		ret->current = argv[ret->argc-1] + 1;
	}
	
	//NextArgv(ret, argc, argv);

	if (ret->argc > argc){
		ret->argc = argc;
		return CleanReturn(ret);
	}

	if (argv[ret->argc-1][0] != '-' || strlen(argv[ret->argc-1]) < 2)
		return CleanReturn(ret);

	if (argv[ret->argc-1][1] == '-'){
		if (argv[ret->argc-1][2] == '\0'){
			NextArgv(ret, argc, argv);
			return CleanReturn(ret);
		}

		return ParseLongOption(cli, ret, argc, argv);

	}
	return ParseOption(cli, ret, argc, argv);
	
}
