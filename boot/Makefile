all:$(OUTPUT)/boot/sldr.bin $(OUTPUT)/fs/sys/start

$(OUTPUT)/boot/sldr0.bin:sldr0.asm display.inc drive.inc std.inc
	$(AS16) -o $@ $<

$(OUTPUT)/boot/sldr1.bin:sldr1.c defines.h display.h drive.h fs.h keyboard.h memory.h time.h
	$(CC16) -o $(OUTPUT)/boot/sldr1.o $<
	$(LD16_1) -o $@ $(OUTPUT)/boot/sldr1.o $(LD16_2)
	rm $(OUTPUT)/boot/sldr1.o

$(OUTPUT)/boot/sldr.bin:$(OUTPUT)/boot/sldr0.bin $(OUTPUT)/boot/sldr1.bin
	cat $(OUTPUT)/boot/sldr0.bin $(OUTPUT)/boot/sldr1.bin > $@

$(OUTPUT)/fs/sys/start:$(OUTPUT)/fs/sys/start16 $(OUTPUT)/fs/sys/start64
	cat $(OUTPUT)/fs/sys/start16 $(OUTPUT)/fs/sys/start64 > $@
	rm $(OUTPUT)/fs/sys/start16
	rm $(OUTPUT)/fs/sys/start64

$(OUTPUT)/fs/sys/start16:start16.asm
	mkdir -p $(OUTPUT)/fs/sys
	$(AS16) -o $@ $<

$(OUTPUT)/fs/sys/start64:$(OUTPUT)/fs/sys/cstart64.o $(OUTPUT)/fs/sys/start64.o $(OUTPUT)/fs/sys/display.o $(OUTPUT)/fs/sys/string.o $(OUTPUT)/fs/sys/io.o	
	$(LD64) -o $@ $(OUTPUT)/fs/sys/cstart64.o $(OUTPUT)/fs/sys/start64.o $(OUTPUT)/fs/sys/display.o $(OUTPUT)/fs/sys/string.o $(OUTPUT)/fs/sys/io.o
	rm $(OUTPUT)/fs/sys/cstart64.o
	rm $(OUTPUT)/fs/sys/start64.o
	rm $(OUTPUT)/fs/sys/display.o
	rm $(OUTPUT)/fs/sys/string.o
	rm $(OUTPUT)/fs/sys/io.o

$(OUTPUT)/fs/sys/cstart64.o:cstart64.asm
	$(AS64) -o $@ $<

$(OUTPUT)/fs/sys/start64.o:start64.c
	$(CC64) -o $@ $<

$(OUTPUT)/fs/sys/display.o:display.asm
	$(AS64) -o $@ $<

$(OUTPUT)/fs/sys/string.o:string.asm
	$(AS64) -o $@ $<

$(OUTPUT)/fs/sys/io.o:io.asm
	$(AS64) -o $@ $<
