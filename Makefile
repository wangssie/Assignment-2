CFLAGS=-Wall -Wno-comment -std=c99

a2: main.o hash.o text_analysis.o
	gcc $(CFLAGS) -o a2 main.o hash.o text_analysis.o

main.o: main.c hash.h text_analysis.h
	gcc $(CFLAGS) -c main.c

hash.o: hash.c hash.h
	gcc $(CFLAGS) -c hash.c

text_analysis.o: text_analysis.c text_analysis.h
	gcc $(CFLAGS) -c text_analysis.c

.PHONY: clean

clean:
	rm *.o
	rm a2
