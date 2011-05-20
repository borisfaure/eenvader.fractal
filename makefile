CC = gcc
DEBUG=yes

ifeq ($(DEBUG),yes)
    CFLAGS = -g -ggdb -Wall -Wextra -std=gnu99
else
    CFLAGS = -Wall -Wextra -O2 -std=gnu99
endif
LDFLAGS = `pkg-config --libs --cflags ecore-evas`
IFLAGS_EXEC  = `pkg-config --cflags ecore-evas`
IFLAGS_SO  = `pkg-config --cflags edje`

EXEC = eenvaders
EXEC_SRC= src/eenvaders.c
EXEC_OBJ= $(EXEC_SRC:.c=.o)

SO = eenvaders.so
SO_SRC= src/eenvaders_edje_external.c

EDJ = eenvaders.edj
EDJ_SRC = data/eenvaders.edc

all: $(EXEC) $(SO) $(EDJ)

$(EDJ): $(EDJ_SRC)
	edje_cc $(EDJ_SRC) $(EDJ)

$(SO): $(SO_SRC)
	$(CC) -fPIC $(CFLAGS) $(IFLAGS_SO) -c src/eenvaders_edje_external.c -o src/eenvaders_edje_external.o
	$(CC) -shared -Wl,-soname,$(SO) -o $(SO) src/eenvaders_edje_external.o

$(EXEC): $(EXEC_SRC)
	$(CC) -o src/eenvaders.o -c src/eenvaders.c $(CFLAGS) $(IFLAGS_EXEC)
	$(CC) $(EXEC_OBJ) -o $(EXEC) $(LDFLAGS)

clean :
	rm eenvaders eenvaders.so eenvaders.edj src/*.o

