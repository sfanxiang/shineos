#CC64 = gcc -c
#OBJCOPY = objcopy -S -j .text -O binary

OUTPUT = output
VM = $(OUTPUT)/vm

all:_boot/sldr.bin $(VM)/hd0

_boot/sldr.bin:
	cd boot && make

$(VM)/hd0:boot/sldr.bin
	dd if=$< of=$@ conv=notrunc
	
tool:
	cd tools && make

