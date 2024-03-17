CC = gcc
CFLAGS = -Wall -Wextra -O3

SOURCES = main.c affichage.c noeud.c minimax.c puissance4.c
EXECUTABLE = puissance4.exe

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCES)

clean:
	rm -f $(EXECUTABLE)
