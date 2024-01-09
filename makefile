CC = gcc
CFLAGS = -Wall -Wextra -O3 -lm
GTKFLAGS = $(shell pkg-config --cflags --libs gtk+-3.0)
puissance4: gui.c
	$(CC) $(CFLAGS) $(GTKFLAGS) -o gui gui.c

clean:
	rm -f puissance4
