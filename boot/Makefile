all:$(OUTPUT)/boot/sldr.bin $(OUTPUT)/fs/sys/start

$(OUTPUT)/boot/sldr0.bin:sldr0.asm display.inc drive.inc std.inc
	$(ASM) -o $@ $<

$(OUTPUT)/boot/sldr1.bin:sldr1.c defines.h display.h drive.h fs.h keyboard.h memory.h time.h
	$(CC16) -o $(OUTPUT)/boot/sldr1.o $<
	$(LD16_1) -o $@ $(OUTPUT)/boot/sldr1.o $(LD16_2)
	rm $(OUTPUT)/boot/sldr1.o

$(OUTPUT)/boot/sldr.bin:$(OUTPUT)/boot/sldr0.bin $(OUTPUT)/boot/sldr1.bin
	cat $(OUTPUT)/boot/sldr0.bin $(OUTPUT)/boot/sldr1.bin > $@

$(OUTPUT)/fs/sys/start:start.c defines.h memory.h
	mkdir -p $(OUTPUT)/fs/sys
	$(CC16) -o $(OUTPUT)/fs/sys/start.o $<
	$(LD16_1) -o $@ $(OUTPUT)/fs/sys/start.o $(LD16_2)
	rm $(OUTPUT)/fs/sys/start.o
