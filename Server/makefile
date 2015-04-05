SOURCES := main.o net.o worker.o data.o

CFLAGS := -std=c99 -lpthread

all: $(SOURCES)
	cc $(SOURCES) -lpthread -o Server

clean:
	rm *.o Server