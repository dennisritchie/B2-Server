CC=gcc
CFLAGS=
DEPS = 
LIBS=-lpthread
TARGET=client

$(TARGET): main.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) 
.PHONY: clean
clean:
	rm -rf *.o $(TARGET)
