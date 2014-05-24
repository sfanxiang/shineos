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

boot : $(TEMP)/part0 $(TEMP)/part1 $(OUTPUT)/hdsample

$(TEMP)/part0 : boot/part0.asm boot/includes/display.inc boot/includes/std.inc boot/includes/hd.inc
	$(NASM) $< -Iboot/includes/ -o $(TEMP)/part0.tmp -E
	$(NASM) $(TEMP)/part0.tmp -o $@

$(TEMP)/part1 : boot/part1.asm
	$(NASM) $< -Iboot/includes/ -o $(TEMP)/part1.tmp -E
	$(NASM) $(TEMP)/part1.tmp -o $@

$(OUTPUT)/hdsample : $(TEMP)/part0 $(TEMP)/part1
	dd if=/dev/zero of=$@ bs=512 count=2048
	dd if=$(TEMP)/part0 of=$@ conv=notrunc
	dd if=$(TEMP)/part1 of=$@ seek=512 oflag=seek_bytes conv=notrunc

init :
	mkdir output
	mkdir temp

clean :
	rm -r $(TEMP)/*
