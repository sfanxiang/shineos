CC16 = bcc -ansi -c -O
LD16_1 = ld86
LD16_2 = -d -L/usr/lib/bcc/ -lc

sldr1.bin : main.o
	$(LD16_1) -o $@ $< $(LD16_2)

main.o : main.c defines.h display.h drive.h fs.h keyboard.h memory.h time.h
	$(CC16) -o $@ $<
