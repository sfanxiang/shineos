#CC64 = gcc -c
#OBJCOPY = objcopy -S -j .text -O binary

export OUTPUT = $(abspath output)
export VM = $(abspath $(OUTPUT)/vm)
export FS = $(abspath $(OUTPUT)/fs)

export ASM = nasm
export CC16 = bcc -ansi -c -O
export LD16_1 = ld86
export LD16_2 = -d -L/usr/lib/bcc/ -lc

all:cleanfs build $(VM)/hd0

cleanfs:
	rm -r $(FS)
	
build:
	cd boot && make

tool:
	cd tools && make

$(VM)/hd0:boot/sldr.bin
	dd if=boot/sldr.bin of=$@ conv=notrunc
