CC16 = bcc -ansi -c
LD16_1 = ld86
LD16_2 = -d -L/usr/lib/bcc/ -lc
sldr1.o : main.c display.h drive.h fs.h memory.h types.h
	$(CC16) -O -o sldr1.tmp $<
	$(LD16_1) -o $@ sldr1.tmp $(LD16_2)
	rm sldr1.tmp
