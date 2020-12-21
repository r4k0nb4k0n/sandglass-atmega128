CC = avr-gcc
CFLAGS = -g -Os -mmcu=atmega128
OBJCOPY = avr-objcopy
OBJFLAGS = -j .text -j .data -O ihex
SRC = main.c
OBJ = main.o
ELF = main.elf
HEX = main.hex

all:
	$(CC) $(CFLAGS) -o $(OBJ) -c $(SRC)
	$(CC) $(CFLAGS) -o $(ELF) $(OBJ)
	$(OBJCOPY) $(OBJFLAGS) $(ELF) $(HEX)
flash:
	#avrdude -c stk500v2 -P /dev/tty.SLAB_USBtoUART -p atmega128 -U flash:w:main.hex:i
	avrdude -c stk500v2 -P /dev/tty.usbserial-0001 -p atmega128 -U flash:w:main.hex:i
screen:
	#screen /dev/tty.SLAB_USBtoUART
	screen /dev/tty.usbserial-0001
clean:
	rm -f main.o main.elf
fclean: clean
	rm -f main.hex 
