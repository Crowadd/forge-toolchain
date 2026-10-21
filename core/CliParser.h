enum argument {NO_ARGUMENT, ARGUMENT_REQUIRED};

struct CliOption {
	char option;
	char *longOption;
	enum argument arg;
};

struct CliReturn {
	char option;
	char *argument;
	char *current;
	int argc;
};

struct CliReturn CliParse(struct CliOption cli[], struct CliReturn *ret, int argc, char *argv[]);

