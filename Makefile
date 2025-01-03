TARGET = redblack-test

CC = gcc

CFLAGS += -Wall -Wextra -pedantic -Wshadow -Werror
CFLAGS += -O3
# CFLAGS += -DDEBUG

LDFLAGS += 

SOURCE = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SOURCE))

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	@rm -rf *.o
	@rm -rf $(OBJS)
	@rm -rf $(TARGET)

