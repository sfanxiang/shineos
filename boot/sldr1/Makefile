ASM = nasm -f obj
CC16 = wcc -d0 -s
LD = wlink format raw bin libpath $(WATCOM)/lib286/dos/ libpath $(WATCOM)/lib286/

sldr1.bin : cstart.o main.o
	$(LD) file cstart.o file main.o name sldr1.bin

cstart.o : cstart.asm
	$(ASM) -o $@ $<

main.o : main.c display.h drive.h fs.h memory.h types.h
	$(CC16) $<
