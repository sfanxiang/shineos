OBJCOPY = objcopy
OBJCOPYPARAM = -S -j .text -O binary
GCC = gcc
GCC_IN = -c 
GCC_OUT = -o 
NASM = nasm
NASM_IN = -f elf64 
NASM_OUT = -o 
OUTPUT = output
TEMP = temp

$(OUTPUT)/booter : boot/booter.asm boot/includes/mbrdata.inc boot/includes/std.inc boot/includes/display.inc boot/includes/hdd.inc boot/includes/string.inc
	$(NASM) boot/booter.asm -Iboot/includes/ -o $(TEMP)/booter.tmp -E
	$(NASM) $(TEMP)/booter.tmp -o $(OUTPUT)/booter

clean :
	rm -r $(TEMP)/*
