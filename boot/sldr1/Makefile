$(OUTPUT)/boot/sldr1.bin : $(OUTPUT)/boot/sldr1.o
	$(LD16_1) -o $@ $< $(LD16_2)
	rm $<

$(OUTPUT)/boot/sldr1.o : main.c defines.h display.h drive.h fs.h keyboard.h memory.h time.h
	$(CC16) -o $@ $<
