all:$(OUTPUT)/boot/sldr.bin $(OUTPUT)/fs/sys/start

$(OUTPUT)/boot/sldr0.bin:sldr0.asm display.inc drive.inc std.inc
	$(ASM) -o $@ $<

$(OUTPUT)/boot/sldr1.bin:sldr1.c defines.h display.h drive.h fs.h keyboard.h memory.h time.h
	$(CC16) -o $(OUTPUT)/boot/sldr1.o $<
	$(LD16_1) -o $@ $(OUTPUT)/boot/sldr1.o $(LD16_2)
	rm $(OUTPUT)/boot/sldr1.o

$(OUTPUT)/boot/sldr.bin:$(OUTPUT)/boot/sldr0.bin $(OUTPUT)/boot/sldr1.bin
	cat $(OUTPUT)/boot/sldr0.bin $(OUTPUT)/boot/sldr1.bin > $@

$(OUTPUT)/fs/sys/start:$(OUTPUT)/fs/sys/start16
	mv $< $@

$(OUTPUT)/fs/sys/start16:start16.asm
	mkdir -p $(OUTPUT)/fs/sys
	$(ASM) -o $@ $<
