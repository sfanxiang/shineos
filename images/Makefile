all:$(OUTPUT)/images/kernel $(OUTPUT)/images/hd0 _writefs

$(OUTPUT)/images/kernel:$(OUTPUT)/kernel/kernel
	cp $< $@

$(OUTPUT)/images/hd0:$(OUTPUT)/boot/sldr.bin
	dd if=/dev/zero of=$@ bs=1024 count=1024
	dd if=$< of=$@ conv=notrunc

_writefs:
	$(OUTPUT)/tools/writefs $(OUTPUT)/images/hd0 $(OUTPUT)/fs 0 2 64
