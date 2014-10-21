all:$(OUTPUT)/boot/sldr.bin $(OUTPUT)/fs/sys/start

$(OUTPUT)/boot/sldr0.bin:sldr0.asm display.inc drive.inc std.inc
	$(ASM) -o $@ $<

$(OUTPUT)/boot/sldr1.bin:sldr1.c defines.h display.h drive.h fs.h keyboard.h memory.h time.h
	$(CC16) -o $(OUTPUT)/boot/sldr1.o $<
	$(LD16_1) -o $@ $(OUTPUT)/boot/sldr1.o $(LD16_2)
	rm $(OUTPUT)/boot/sldr1.o

$(OUTPUT)/boot/sldr.bin:$(OUTPUT)/boot/sldr0.bin $(OUTPUT)/boot/sldr1.bin
	cat $(OUTPUT)/boot/sldr0.bin $(OUTPUT)/boot/sldr1.bin > $@

$(OUTPUT)/fs/sys/start:$(OUTPUT)/fs/sys/start16 $(OUTPUT)/fs/sys/start32 $(OUTPUT)/fs/sys/bssspace
	cat $(OUTPUT)/fs/sys/start16 $(OUTPUT)/fs/sys/bssspace $(OUTPUT)/fs/sys/start32 $(OUTPUT)/fs/sys/bssspace > $(OUTPUT)/fs/sys/start
	rm $(OUTPUT)/fs/sys/start16
	rm $(OUTPUT)/fs/sys/start32
	rm $(OUTPUT)/fs/sys/bssspace

$(OUTPUT)/fs/sys/start16:$(OUTPUT)/fs/sys/cstart16.o $(OUTPUT)/fs/sys/start16.o $(OUTPUT)/fs/sys/cend16.o
	$(LD16_1) -o $@ $(OUTPUT)/fs/sys/cstart16.o $(LD16_2) $(OUTPUT)/fs/sys/start16.o $(OUTPUT)/fs/sys/cend16.o
	rm $(OUTPUT)/fs/sys/cstart16.o
	rm $(OUTPUT)/fs/sys/start16.o
	rm $(OUTPUT)/fs/sys/cend16.o

$(OUTPUT)/fs/sys/cstart16.o:cstart16.asm
	mkdir -p $(OUTPUT)/fs/sys
	$(ASM) -f as86 -o $@ $<

$(OUTPUT)/fs/sys/start16.o:start16.c defines.h memory.h
	mkdir -p $(OUTPUT)/fs/sys
	$(CC16) -o $@ $<

$(OUTPUT)/fs/sys/cend16.o:cend16.asm
	mkdir -p $(OUTPUT)/fs/sys
	$(ASM) -f as86 -o $@ $<

$(OUTPUT)/fs/sys/start32:$(OUTPUT)/fs/sys/start32.o
	$(LD32_1) -o $@ $< $(LD32_2)
	rm $(OUTPUT)/fs/sys/start32.o

$(OUTPUT)/fs/sys/start32.o:start32.c
	mkdir -p $(OUTPUT)/fs/sys
	$(CC32) -o $@ $<

$(OUTPUT)/fs/sys/bssspace:bssspace.asm
	mkdir -p $(OUTPUT)/fs/sys
	$(ASM) -o $@ $<
