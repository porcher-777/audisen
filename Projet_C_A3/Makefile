CC = gcc
LDFLAGS = -lm
CFLAGS = -Wall -Wextra
EXEC = audisen_usb audisen_sim
LIB = ftd2xx.lib

all:$(EXEC)

audisen_usb : audisen_usb.o amp.o ams.o frame.o usb.o autotests.o
	$(CC) -g -o $@ $^ $(LDFLAGS) $(LIB)

audisen_sim : audisen_sim.o amp.o ams.o frame.o usb.o autotests.o
	$(CC) -g -o $@ $^ $(LDFLAGS) $(LIB)

%.o: %.c
	$(CC) -g -o $@ -c $< $(CFLAGS)

clean:
	rm *.o

mrproper: clean
	rm $(EXEC)