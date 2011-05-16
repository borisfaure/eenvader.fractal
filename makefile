CC = gcc
DEBUG=yes
#X11 or EFL
GUI=EFL

ifeq ($(DEBUG),yes)
    CFLAGS = -DDEBUG -g -ggdb -Wall -Wextra -pg
else
    CFLAGS = -Wall -Wextra -O2
endif
LDFLAGS = -lm -pg `pkg-config --libs --cflags ecore-evas`

EXEC = eenvaders

SRC= $(wildcard src/*.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean :
	rm $(OBJ)

