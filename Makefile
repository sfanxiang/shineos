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
VM = $(OUTPUT)/vm

.PHONY : boot

boot : $(TEMP)/sldr $(VM)/hd0

$(TEMP)/sldr : boot/sldr.asm boot/includes/display.inc boot/includes/std.inc boot/includes/hd.inc boot/includes/fs.inc boot/includes/string.inc
	$(NASM) $< -Iboot/includes/ -o $(TEMP)/sldr.tmp -E
	$(NASM) $(TEMP)/sldr.tmp -o $@

$(VM)/hd0 : $(TEMP)/sldr
	dd if=$(TEMP)/sldr of=$@ conv=notrunc

init :
	mkdir $(TEMP)

clean :
	rm -r $(TEMP)/*
