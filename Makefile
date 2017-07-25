# Makefile template for shared library

CC = gcc # C compiler
CFLAGS = -fPIC -Wall -Wextra -O2 -g  -lpthread # C flags
LDFLAGS = -shared  # linking flags
RM = rm -f  # rm command
TARGET_LIB = libJuliaMcast.so # target lib

SRCS = msock.c juliaMcast.c
OBJS = $(SRCS:.c=.o)

.PHONY: all
all: ${TARGET_LIB} server client send receive

$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^
	
server: server.o msock.o
	$(CC) -o server server.c msock.c
	
client: client.o msock.o
	$(CC) -o client client.c msock.c

send: send.o
	$(CC) -o send send.c

receive: receive.o
	$(CC) -o receive receive.c

$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

include $(SRCS:.c=.d)

.PHONY: clean
clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d)
	-${RM} server client
