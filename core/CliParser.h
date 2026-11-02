#ifndef CLIPARSER_H
#define CLIPARSER_H

typedef struct CliOption {
	char option;
	const char *longOption;
	int retOption;
	enum {NO_ARGUMENT, ARGUMENT_REQUIRED} arg;
} CliOption;

typedef struct CliReturn {
	int retOption;
	const char *argument;
	const char *current;
	int argc;
} CliReturn;

CliReturn CliParse(CliOption cli[], CliReturn *ret, int argc, const char *argv[]);

#endif
