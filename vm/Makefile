all:$(OUTPUT)/vm/bochsrc $(OUTPUT)/vm/hd0 _writefs

$(OUTPUT)/vm/bochsrc:bochsrc
	cp $< $@

$(OUTPUT)/vm/hd0:$(OUTPUT)/boot/sldr.bin
	dd if=/dev/zero of=$@ bs=1024 count=1024
	dd if=$< of=$@ conv=notrunc

_writefs:
	$(OUTPUT)/tools/writefs $(OUTPUT)/vm/hd0 $(OUTPUT)/fs 0 2 64
