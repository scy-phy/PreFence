CC=gcc
CFLAGS=-Wall
DEPS = ../ctrlpf_ioctl.h
OBJ = ioctl.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ioctl: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
