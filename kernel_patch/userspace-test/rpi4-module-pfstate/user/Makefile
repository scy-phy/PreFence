CC=gcc
CFLAGS=-Wall
DEPS = ../pfstate_ioctl.h
OBJ = ioctl.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ioctl: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
