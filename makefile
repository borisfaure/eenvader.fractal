CC = gcc
DEBUG=yes

ifeq ($(DEBUG),yes)
    CFLAGS = -g -ggdb -Wall -Wextra -std=gnu99
else
    CFLAGS = -Wall -Wextra -O2 -std=gnu99
endif
LDFLAGS = -lm `pkg-config --libs --cflags ecore-evas`
IFLAGS_EXEC  = `pkg-config --cflags ecore-evas`
IFLAGS_SO  = `pkg-config --cflags edje`

EXEC = eenvaders
EXEC_SRC= src/eenvaders.c
EXEC_OBJ= $(EXEC_SRC:.c=.o)

SO = eenvaders.so
SO_SRC= src/eenvaders_edje_external.c
SO_OBJ= $(SO_SRC:.c=.o)

all: $(EXEC) $(SO)

$(SO): $(SO_SRC)
	$(CC) -Wall -fPIC $(CFLAGS) $(IFLAGS_SO) -c $(SO_SRC) -o $(SO_OBJ)
	$(CC) -shared -Wl,-soname,$(SO) -o $(SO) $(SO_OBJ)

$(EXEC): $(EXEC_SRC)
	$(CC) -o $@ -c $(EXEC_SRC) $(CFLAGS) $(IFLAGS_EXEC)
	$(CC) $(EXEC_OBJ) -o $(EXEC) $(LDFLAGS)

clean :
	rm $(EXEC_OBJ) $(SO_OBJ)

