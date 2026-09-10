CC = gcc

VERSION ?= 0.0.0

CFLAGS = -Wall -Wextra -std=c17 -MMD -MP
CPPFLAGS = -DFORGE_CC_VERSION=\"$(VERSION)\"

TARGET = forge-cc

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
DEPENDS = $(OBJECTS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

-include $(DEPENDS)

clean:
	rm -f $(OBJECTS) $(DEPENDS) $(TARGET)


