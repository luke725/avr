all: main.hex

%.o: %.c
	avr-gcc -g -Os -mmcu=atmega8 -c $< -o $@

%.elf: %.o lib/leds.o lib/time.o
	avr-gcc -g -mmcu=atmega8 -o $@ $^

%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex $< $@
