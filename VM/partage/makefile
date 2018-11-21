CC=gcc
EXEC=tunnel64d

all: $(EXEC)

tunnel64d : main.o iftun.o extremite.o
	$(CC) -o $@ $^

iftun.o: iftun.c iftun.h
	$(CC) -o $@ -c $<
	
extremite.o: extremite.c extremite.h
	$(CC) -o $@ -c $<

main.o: main.c iftun.h extremite.h
	$(CC) -o $@ -c $<

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
