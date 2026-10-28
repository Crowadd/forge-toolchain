# File Management API Reference

A lightweight C utility module for loading binary/text files entirely into memory buffers, tracking byte read cursors and text encodings, and persisting memory buffers to disk.

---

## Table of Contents

1. [Overview](#overview)
2. [Data Types and Structures](#data-types-and-structures)
   - [Encoding](#encoding)
   - [FileInfo](#fileinfo)
3. [Function Reference](#function-reference)
   - [CopyFile](#copyfile)
   - [CreateFile](#createfile)
4. [Memory Layout](#memory-layout)
5. [Usage Example](#usage-example)
6. [Dependencies and Prerequisites](#dependencies-and-prerequisites)
7. [Caveats & Best Practices](#caveats--best-practices)

---

## Overview

The file management interface provides an abstraction for whole-file I/O operations:

- **Slurping files into memory:** Reads an entire file into a contiguous, heap-allocated buffer with a null-terminator appended for safe string traversal.
- **Cursor tracking:** Stores base, current, and sentinel pointers (`fileData`, `current`, `fileEnd`) to simplify linear parsing.
- **Writing to disk:** Writes an in-memory buffer back to disk using the file path associated with the `FileInfo` struct.

---

## Data Types and Structures

### `Encoding`

An enumeration representing character encodings supported across file processing workflows.

```c
typedef enum Encoding {
    UTF_8,
    UTF_16LE,
    UTF_16BE
} Encoding;
```

| Enumerator | Description |
| :--- | :--- |
| `UTF_8` | Standard 8-bit variable-length Unicode transformation format. |
| `UTF_16LE` | 16-bit Unicode transformation format (Little-Endian byte order). |
| `UTF_16BE` | 16-bit Unicode transformation format (Big-Endian byte order). |

---

### `FileInfo`

A state structure encapsulating file buffer pointers, buffer metadata, and configuration.

```c
typedef struct FileInfo {
    char     *filePath;
    uint8_t  *fileData;
    uint8_t  *fileEnd;
    uint8_t  *current;
    Encoding  encType;
} FileInfo;
```

| Field | Type | Description |
| :--- | :--- | :--- |
| `filePath` | `char*` | Pointer to the null-terminated string holding the file path. |
| `fileData` | `uint8_t*` | Pointer to the beginning of the heap-allocated file buffer. |
| `fileEnd` | `uint8_t*` | Sentinel pointer pointing to the byte directly after the last file byte (`fileData + fileSize`). |
| `current` | `uint8_t*` | Traversal cursor for stream or token parsing. Initialized to `fileData`. |
| `encType` | `Encoding` | Designates the file encoding. (Note: Not populated automatically by `CopyFile`; managed by the caller). |

---

## Function Reference

### `CopyFile`

Loads the target file entirely into heap memory, sets tracking cursors, and appends a trailing null byte.

```c
int CopyFile(FileInfo *fileInfo, char *filePath);
```

#### Parameters

- **`fileInfo`**: Pointer to a caller-allocated `FileInfo` structure to populate.
- **`filePath`**: Path to the file on disk.

#### Return Value

- **`0`**: Success. `fileInfo` fields are populated, and the buffer is ready.
- **`1`**: Open failed (e.g., file not found or permission denied via `xfopen`).

#### Behavioral Details

1. Opens the file in binary read mode (`"rb"`).
2. Calculates file size via `xfseek(..., SEEK_END)` and `xftell(...)`.
3. Allocates `fileSize + 1` bytes via `xmalloc`.
4. Reads the entire file into `fileInfo->fileData`. If read fails, calls `perror()`, frees the buffer, and invokes `exit(EXIT_FAILURE)`.
5. Sets `fileEnd = fileData + fileSize`, `current = fileData`, and appends `'\0'` at `fileData[fileSize]`.
6. Retains the reference `fileInfo->filePath = filePath`.
7. Closes the file handle with `xfclose`.

---

### `CreateFile`

Writes the memory buffer described in `fileInfo` to the path located at `fileInfo.filePath`.

```c
void CreateFile(FileInfo fileInfo);
```

#### Parameters

- **`fileInfo`**: A `FileInfo` structure containing valid `filePath`, `fileData`, and `fileEnd` fields. Passed by value.

#### Behavioral Details

1. Opens destination path in binary write mode (`"wb"`). If failed, invokes `exit(EXIT_FAILURE)`.
2. Computes write length using pointer arithmetic: `fileSize = fileInfo.fileEnd - fileInfo.fileData`.
3. Writes the block to disk with `xfwrite`.
4. Closes the file handle with `xfclose`.

---

## Memory Layout

When `CopyFile` populates a `FileInfo` struct for a file of size $N$:

```text
fileData                            fileEnd
  |                                    |
  v                                    v
+---+---+---+---+ ... +---+---+---+---+----+
| 0 | 1 | 2 | 3 |     |N-3|N-2|N-1| N |'\0'|
+---+---+---+---+ ... +---+---+---+---+----+
  ^                                    ^
  |                                    |
current                        Extra null byte
(starts at index 0)            (index N)
```

- Buffer length in bytes: $N$
- Total allocated memory: $N + 1$ bytes
- Valid data slice: `[fileData, fileEnd)`

---

## Usage Example

```c
#include <stdio.h>
#include <stdlib.h>
#include "file.h"

int main(void) {
    FileInfo info = {0};

    // 1. Read file into memory
    if (CopyFile(&info, "source.txt") != 0) {
        fprintf(stderr, "Could not open source file.\n");
        return EXIT_FAILURE;
    }

    printf("Loaded %td bytes from '%s'\n", info.fileEnd - info.fileData, info.filePath);

    // 2. Inspect / manipulate the buffer
    while (info.current < info.fileEnd) {
        if (*info.current == '\n') {
            // Found newline
        }
        info.current++;
    }

    // 3. Write buffer to a new file
    info.filePath = "destination_copy.txt";
    CreateFile(info);

    // 4. Clean up allocated buffer
    free(info.fileData);

    return EXIT_SUCCESS;
}
```

---

## Dependencies and Prerequisites

This implementation relies on wrapped standard I/O functions declared in `"core.h"`:

- `xfopen(const char *filename, const char *mode)`: Wrapper around `fopen`
- `xfseek(FILE *stream, long offset, int whence)`: Wrapper around `fseek`
- `xftell(FILE *stream)`: Wrapper around `ftell`
- `xfread(void *ptr, size_t size, size_t nmemb, FILE *stream)`: Wrapper around `fread`
- `xfwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)`: Wrapper around `fwrite`
- `xfclose(FILE *stream)`: Wrapper around `fclose`
- `xmalloc(size_t size)`: Wrapper around `malloc`

---

## Caveats & Best Practices

1. **Manual Cleanup:** `fileInfo->fileData` is allocated using `xmalloc`. The caller is responsible for freeing this pointer via `free(fileInfo.fileData)` to avoid memory leaks once processing is complete.
2. **Path Lifetime:** `CopyFile` stores `fileInfo->filePath = filePath` by pointer without copying the string. Ensure that the source `filePath` string buffer remains in scope and is not modified or deallocated during the lifetime of `fileInfo`.
3. **Encoding Field:** `CopyFile` does not inspect or initialize `fileInfo->encType`. Set this explicitly after loading if character encoding tracking is needed.