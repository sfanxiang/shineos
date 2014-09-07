all:$(OUTPUT)/vm/bochsrc $(OUTPUT)/vm/hd0

$(OUTPUT)/vm/bochsrc:bochsrc
	cp $< $@

$(OUTPUT)/vm/hd0:$(OUTPUT)/boot/sldr.bin
	dd if=/dev/zero of=$@ bs=1024 count=1024
	dd if=$< of=$@ conv=notrunc
