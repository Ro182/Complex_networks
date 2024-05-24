CC=gcc
CFLAGS=-I/opt/local/include
LDFLAGS=-L/opt/local/lib
LIBS=-lgsl
SRCS=$(shell find . -name "*.c")
PROGS=$(patsubst %.c,%,$(SRCS))



all: $(PROGS)

%: %.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS) $(LIBS)

mkdir:
	mkdir -p 1_Data_example

clean:
	rm -f $(PROGS)
	rm -rf 1_Data_example

clean_all_data:
	rm -rf **/*.dat
