CC16 = bcc -ansi -c
LD16_1 = ld86
LD16_2 = -d -L/usr/lib/bcc/ -lc

sldr1.bin : main.o
	$(LD16_1)  -o $@ main.o $(LD16_2)

main.o : main.c display.h drive.h fs.h memory.h types.h
	 $(CC16) -O -o $@ $<
