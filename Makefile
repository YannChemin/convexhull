CC = gcc
CFLAGS = -Wall

SRC = convex_hull.c
OBJ = convex_hull.o

all: convex_hull

convex_hull: $(OBJ)
	$(CC) $(CFLAGS) -o convex_hull $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

clean:
	rm -f convex_hull $(OBJ)

