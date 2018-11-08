CC=gcc
EXEC=tun

all: $(EXEC)

tun: main.o iftun.o
	$(CC) -o $@ $^

iftun.o: iftun.c iftun.h
	$(CC) -o $@ -c $<

main.o: main.c iftun.h
	$(CC) -o $@ -c $<

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
