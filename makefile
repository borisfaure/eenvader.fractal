CC = gcc
DEBUG=yes

ifeq ($(DEBUG),yes)
    CFLAGS = -g -ggdb -Wall -Wextra -std=gnu99
else
    CFLAGS = -Wall -Wextra -O2 -std=gnu99
endif
LDFLAGS = -lm `pkg-config --libs --cflags ecore-evas`
IFLAGS  = `pkg-config --cflags ecore-evas`
IFLAGS_SO  = `pkg-config --cflags edje`

EXEC = eenvaders
SO = eenvaders.so
SRC= eenvaders.c
OBJ= $(SRC:.c=.o)

all: $(EXEC) $(SO)

$(SO): eenvaders_edje_external.c
	$(CC) -Wall -fPIC $(CFLAGS) $(IFLAGS_SO) -c eenvaders_edje_external.c
	$(CC) -shared -Wl,-soname,eenvaders.so -o eenvaders.so eenvaders_edje_external.o

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(IFLAGS)

clean :
	rm $(OBJ)

