CC=gcc
CFLAGS= ../../def.h ../../gbv.h 
DEPS = ./send/%.h ./recv/%.h
LIBS=-lpthread -lrt
TARGET=message.o
OBJ= send_twin.o recv_twins.o
%.o: ./send/%.c ./recv/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$TARGET: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -rf *.o
