EXECBIN  = life

CC       = clang
CFLAGS   = -Wall -Wpedantic -Werror -Wextra
LIBFLAGS = -lncurses

SOURCES  = $(wildcard *.c)
OBJECTS  = $(SOURCES:%.c=%.o)

.PHONY: all clean

all: $(EXECBIN)

$(EXECBIN): $(OBJECTS)
	$(CC) -o $@ $^ $(LIBFLAGS)

clean:
	rm -f $(EXECBIN) $(OBJECTS)

%.o : %.c
	$(CC) $(CFLAGS) -c $<

format:
	clang-format -i -style=file *.[ch]
