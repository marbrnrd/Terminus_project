# Makefile

CPPFLAGS = -MMD
CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS =
LDLIBS =

SRC = echo.c echo_test.c fork_server.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: echo_test fork_server

echo_test: echo_test.o echo.o
fork_server: fork_server.o echo.o

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} echo_test fork_server

# END
