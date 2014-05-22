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

boot : $(TEMP)/mbr $(OUTPUT)/hdsample

$(TEMP)/mbr : boot/mbr.asm boot/includes/display.inc boot/includes/std.inc boot/includes/hd.inc
	$(NASM) $< -Iboot/includes/ -o $@

$(OUTPUT)/hdsample : $(TEMP)/mbr
	dd if=/dev/zero of=$@ bs=512 count=2048
	dd if=$< of=$@ conv=notrunc

init :
	mkdir output
	mkdir temp

clean :
	rm -r $(TEMP)/*
