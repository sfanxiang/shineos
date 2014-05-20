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

.PHONY : boot

boot : $(TEMP)/booter $(TEMP)/loader $(OUTPUT)/bootloader

$(TEMP)/booter : boot/booter.asm boot/includes/mbrdata.inc boot/includes/std.inc boot/includes/display.inc
	$(NASM) boot/booter.asm -Iboot/includes/ -o $@.tmp -E
	$(NASM) $@.tmp -o $@


$(TEMP)/loader : boot/loader.asm boot/includes/mbrdata.inc boot/includes/std.inc boot/includes/display.inc boot/includes/hdd.inc boot/includes/string.inc
	$(NASM) boot/loader.asm -Iboot/includes/ -o $@.tmp -E
	$(NASM) $@.tmp -o $@

$(OUTPUT)/bootloader : $(TEMP)/booter $(TEMP)/loader
	cat $(TEMP)/booter $(TEMP)/loader > $@

init :
	mkdir output
	mkdir temp

clean :
	rm -r $(TEMP)/*
