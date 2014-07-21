
#CC64 = gcc -c
#OBJCOPY = objcopy -S -j .text -O binary

OUTPUT = output
#TEMP = temp
VM = $(OUTPUT)/vm

all : make_boot $(VM)/hd0

make_boot :
	cd boot && make

$(VM)/hd0 : boot/sldr.o
	dd if=$< of=$@ conv=notrunc
	
tool :
	cd tools && make

#init :
#	mkdir $(TEMP)
#
#clean :
#	rm -r $(TEMP)/*

