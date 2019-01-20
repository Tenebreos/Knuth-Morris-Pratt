CC			= g++
CFLAGS		= -Wall -Wextra -g -std=gnu++17 -static

OBJ			= main.o func.o

EXEC_NAME	= KnuthMorrisPratt

KMP:	$(OBJ)
	$(CC) -o $(EXEC_NAME) $(OBJ) $(CFLAGS)

main.o:	main.cpp kmp.h
	$(CC) -c main.cpp $(CFLAGS)

func.o: func.cpp kmp.h
	$(CC) -c func.cpp $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.h.gch
