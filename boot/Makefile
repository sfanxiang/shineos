all:$(OUTPUT)/boot/sldr.bin $(OUTPUT)/boot/start

$(OUTPUT)/boot/sldr0.bin:sldr0.asm display.inc drive.inc std.inc
	$(AS16) -o $@ $<

$(OUTPUT)/boot/sldr1.bin:sldr1.c defines.h display.h drive.h fs.h keyboard.h memory.h time.h
	$(CC16) -o $(OUTPUT)/boot/sldr1.o $<
	$(LD16_1) -o $@ $(OUTPUT)/boot/sldr1.o $(LD16_2)

$(OUTPUT)/boot/sldr.bin:$(OUTPUT)/boot/sldr0.bin $(OUTPUT)/boot/sldr1.bin
	cat $^ > $@

$(OUTPUT)/boot/start:$(OUTPUT)/boot/start16 $(OUTPUT)/boot/start64
	cat $^ > $@

$(OUTPUT)/boot/start16:start16.asm
	$(AS16) -o $@ $<

$(OUTPUT)/boot/start64:link.ld $(OUTPUT)/boot/crt0.o $(OUTPUT)/boot/start64.o $(OUTPUT)/boot/display.o $(OUTPUT)/boot/string.o $(OUTPUT)/boot/io.o	
	$(LD64) -T link.ld -o $@ $^

$(OUTPUT)/boot/crt0.o:crt0.S
	$(AS64) -o $@ $<

$(OUTPUT)/boot/start64.o:start64.c ahci.h ata.h defines.h drive.h display.h fs.h io.h memory.h pci.h string.h
	$(CC64) -o $@ $<

$(OUTPUT)/boot/display.o:display.S
	$(AS64) -o $@ $<

$(OUTPUT)/boot/string.o:string.S
	$(AS64) -o $@ $<

$(OUTPUT)/boot/io.o:io.S
	$(AS64) -o $@ $<
