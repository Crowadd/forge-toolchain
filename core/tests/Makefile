CC := gcc
CFLAGS := -Wall -Wextra -std=c23
CPPFLAGS := -I..

PARENT_SRC := $(wildcard ../*.c)
TEST_SRC := $(wildcard *.c)
TARGETS := $(TEST_SRC:.c=)

.PHONY: all clean

all: $(TARGETS)

%: %.c $(PARENT_SRC)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DPROGRAM_NAME='"$@"' $^ -o $@

clean:
	rm -f $(TARGETS)
