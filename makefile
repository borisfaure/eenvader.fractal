CC = gcc
DEBUG=yes

ifeq ($(DEBUG),yes)
    CFLAGS = -g -ggdb -Wall -Wextra -std=gnu99
else
    CFLAGS = -Wall -Wextra -O2 -std=gnu99
endif
LDFLAGS = -lm -pg `pkg-config --libs --cflags ecore-evas`
IFLAGS  = `pkg-config --cflags ecore-evas`

EXEC = eenvaders

SRC= eenvaders.c
OBJ= $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(IFLAGS)

clean :
	rm $(OBJ)

