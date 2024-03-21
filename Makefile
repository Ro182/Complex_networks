CC=gcc
CFLAGS=-I/opt/local/include
LDFLAGS=-L/opt/local/lib
LIBS=-lgsl

program: main.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

clean:
	rm -f program
	rm -f links*
	rm -f states*
