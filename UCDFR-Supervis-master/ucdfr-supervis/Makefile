CC=avr-gcc

OPTIONS=-std=c99 -DF_CPU=16000000UL
OPTIMIZE=-O3
WARN=-Wall
MMCU=at90usb1286
MMCU_CODE=usb1286

CFLAGS=$(OPTIMIZE) $(WARN) $(OPTIONS) -mmcu=$(MMCU)

NAME=ucdfr-supervis
OBJECTS=$(NAME).o hardware.o interface.o usb_serial.o

.PHONY: all upload clean



# Rules

all: $(NAME).hex

$(NAME).hex: $(NAME).elf
	avr-objcopy -j .text -j .data -O ihex $(NAME).elf $(NAME).hex



$(NAME).elf: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

*.c:



upload: $(NAME).hex
	teensy_loader_cli -mmcu=at90usb1286 -v -w $(NAME).hex



clean:
	rm -f *.o $(NAME).elf
