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

boot : $(TEMP)/sldr $(OUTPUT)/hdsample

$(TEMP)/sldr : boot/sldr.asm boot/includes/display.inc boot/includes/std.inc boot/includes/hd.inc boot/includes/fs.inc
	$(NASM) $< -Iboot/includes/ -o $(TEMP)/sldr.tmp -E
	$(NASM) $(TEMP)/sldr.tmp -o $@

$(OUTPUT)/hdsample : $(TEMP)/sldr
	dd if=/dev/zero of=$@ bs=512 count=2048
	dd if=$(TEMP)/sldr of=$@ conv=notrunc

init :
	mkdir output
	mkdir temp

clean :
	rm -r $(TEMP)/*
