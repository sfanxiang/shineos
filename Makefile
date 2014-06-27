OBJCOPY = objcopy
OBJCOPYPARAM = -S -j .text -O binary
CC = gcc
CC_IN = -c 
CC_OUT = -o 
ASM = nasm
ASM_IN = -f elf64 
ASM_OUT = -o 

OUTPUT = output
TEMP = temp
VM = $(OUTPUT)/vm

all : boot

boot : $(TEMP)/sldr0 $(TEMP)/sldr1 $(VM)/hd0

$(TEMP)/sldr0 : boot/sldr0.asm boot/display.inc boot/hd.inc boot/std.inc
	$(ASM) $< -Iboot/ -o $(TEMP)/sldr0.tmp -E
	$(ASM) $(TEMP)/sldr0.tmp -o $@

$(TEMP)/sldr1 : boot/sldr1.asm boot/display.inc boot/file.inc boot/fs.inc boot/hd.inc boot/std.inc boot/string.inc
	$(ASM) $< -Iboot/ -o $(TEMP)/sldr1.tmp -E
	$(ASM) $(TEMP)/sldr1.tmp -o $@

$(VM)/hd0 : $(TEMP)/sldr0 $(TEMP)/sldr1
	dd if=$(TEMP)/sldr0 of=$@ conv=notrunc
	dd if=$(TEMP)/sldr1 of=$@ seek=512 oflag=seek_bytes conv=notrunc
	
tool :
	cd tools && make

init :
	mkdir $(TEMP)

clean :
	rm -r $(TEMP)/*

