CFLAGS= -Wall -ansi -O3
wordmorph: main.o list.o quicksort.o readfile.o dif.o dijkstra.o
	gcc $(CFLAGS) -o wordmorph main.o list.o  quicksort.o readfile.o dif.o dijkstra.o

main.o: main.c main.h readfile.h
	gcc $(CFLAGS) -c main.c

list.o: list.c list.h
	gcc $(CFLAGS) -c list.c

quicksort.o: quicksort.c quicksort.h
	gcc $(CFLAGS) -c quicksort.c

readfile.o: readfile.c readfile.h list.h quicksort.h dif.h dijkstra.h
	gcc $(CFLAGS) -c readfile.c

dif.o: dif.c dif.h
	gcc $(CFLAGS) -c dif.c

dijkstra.o: dijkstra.c dijkstra.h list.h
	gcc $(CFLAGS) -c dijkstra.c

clean:
	rm -f *.o *.gch wordmorph
