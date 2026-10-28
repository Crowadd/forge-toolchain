# CliParser API Reference

`CliParser` is a lightweight, zero-allocation C library designed to parse short (`-o`) and long (`--option`) command-line arguments iteratively from `argc` and `argv`.

---

## Table of Contents

1. [Overview](#overview)
2. [Data Types and Structures](#data-types-and-structures)
   - [enum argument](#enum-argument)
   - [struct CliOption](#struct-clioption)
   - [struct CliReturn](#struct-clireturn)
3. [Function Reference](#function-reference)
   - [CliParse](#cliparse)
4. [Usage Example](#usage-example)
5. [Behavior & Parsing Rules](#behavior--parsing-rules)
6. [Notes and Edge Cases](#notes-and-edge-cases)

---

## Overview

The API functions similarly to POSIX `getopt_long`. It operates by maintaining state inside a `struct CliReturn` tracking variable across iterative calls within a loop.

Key features:
- Short flag parsing (e.g., `-v`, `-f file.txt`, `-ffile.txt`)
- Long flag parsing (e.g., `--help`, `--output=file.txt`, `--output file.txt`)
- Double dash (`--`) delimiter parsing
- Zero dynamic memory allocation (`malloc`/`free` are not used)

---

## Data Types and Structures

### `enum argument`

Specifies whether an option expects an associated argument value.

```c
enum argument {
    NO_ARGUMENT,
    ARGUMENT_REQUIRED
};
```

| Enumerator | Description |
|---|---|
| `NO_ARGUMENT` | The option acts as a standalone boolean flag. |
| `ARGUMENT_REQUIRED` | The option requires a value (either separated by whitespace or attached via `=`). |

---

### `struct CliOption`

Defines a single supported command-line option in the parser configuration array.

```c
struct CliOption {
    char option;
    char *longOption;
    enum argument arg;
};
```

| Field | Type | Description |
|---|---|---|
| `option` | `char` | Single character identifier (e.g., `'h'`, `'v'`). An entry with `option = 0` (or `'\0'`) **must** be used as a sentinel to terminate the options array. |
| `longOption` | `char*` | String literal for the long representation (e.g., `"help"`, `"version"`). Do not include leading dashes. |
| `arg` | `enum argument` | Specifies if this option requires a parameter (`NO_ARGUMENT` or `ARGUMENT_REQUIRED`). |

---

### `struct CliReturn`

Maintains the parser's state between calls and yields the result of each parsing step.

```c
struct CliReturn {
    char option;
    char *argument;
    char *current;
    int argc;
};
```

| Field | Type | Description |
|---|---|---|
| `option` | `char` | Output: The parsed option character. Set to `-1` if an unrecognized option was encountered, or `0` when parsing is complete. |
| `argument` | `char*` | Output: Pointer to the option's value (argument string) if `arg == ARGUMENT_REQUIRED`; otherwise `NULL`. |
| `current` | `char*` | Internal state: Cursor tracking the position inside the current `argv` string being evaluated. |
| `argc` | `int` | Internal state: Current index position within the `argv` array. Initialized to `0` by the caller prior to the loop. |

---

## Function Reference

### `CliParse`

Iteratively parses options from the provided arguments list.

```c
struct CliReturn CliParse(struct CliOption *cli, struct CliReturn *ret, int argc, char *argv[]);
```

#### Parameters

- **`cli`**: Pointer to an array of `struct CliOption` structures, terminated by an entry whose `option` field is `0` / `'\0'`.
- **`ret`**: Pointer to the state/accumulator structure `struct CliReturn`. Must have its members zeroed or `ret->argc = 0` before the first call.
- **`argc`**: Argument count passed to `main()`.
- **`argv`**: Argument vector passed to `main()`.

#### Return Value

Returns a copy of the updated `struct CliReturn`:
- `ret.option > 0`: Matches the `option` character of the discovered flag.
- `ret.option == -1`: An unknown/unregistered option was encountered.
- `ret.option == 0`: Parsing complete, end of options, or an unhandled positional argument was encountered.

---

## Usage Example

```c
#include <stdio.h>
#include "CliParser.h"

int main(int argc, char *argv[]) {
    // 1. Define options array terminated with sentinel {0, NULL, 0}
    struct CliOption options[] = {
        {'h', "help",    NO_ARGUMENT},
        {'v', "version", NO_ARGUMENT},
        {'o', "output",  ARGUMENT_REQUIRED},
        {'i', "input",   ARGUMENT_REQUIRED},
        {0,   NULL,      0}
    };

    // 2. Initialize the return/state structure
    struct CliReturn ret = {0};

    // 3. Loop until option returns 0
    while (CliParse(options, &ret, argc, argv).option) {
        switch (ret.option) {
            case 'h':
                printf("Help flag passed.\n");
                break;
            case 'v':
                printf("Version flag passed.\n");
                break;
            case 'o':
                printf("Output target: %s\n", ret.argument ? ret.argument : "(missing)");
                break;
            case 'i':
                printf("Input file: %s\n", ret.argument ? ret.argument : "(missing)");
                break;
            case -1:
                fprintf(stderr, "Unknown option encountered.\n");
                break;
            default:
                break;
        }
    }

    return 0;
}
```

---

## Behavior & Parsing Rules

1. **Short Options (`-`):**
   - Single options: `-v`
   - Bundled short flags: `-vh` (parsed sequentially on repeated calls)
   - Attached values: `-ofilename.txt` sets `argument` to `"filename.txt"`
   - Separated values: `-o filename.txt` sets `argument` to `"filename.txt"`

2. **Long Options (`--`):**
   - Standard flags: `--help`
   - Separated values: `--output filename.txt`
   - Inline key-value assignment: `--output=filename.txt`

3. **Double Dash (`--`):**
   - An argument consisting strictly of `--` halts option parsing cleanly (`ret.option = 0`).

---

## Notes and Edge Cases

- **Sentinel Requirement:** The `cli` array passed into `CliParse` must be null-terminated (`option = 0`), otherwise the option lookup loops will cause out-of-bounds reads.
- **Clean State:** Always zero-initialize the `struct CliReturn` (`struct CliReturn ret = {0};`) before invoking `CliParse` for the first time.