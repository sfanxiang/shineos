all:$(OUTPUT)/fs/boot/sldr.cfg _subdirs $(OUTPUT)/boot/sldr.bin

$(OUTPUT)/boot/sldr.bin:$(OUTPUT)/boot/sldr0.bin $(OUTPUT)/boot/sldr1.bin
	cat $(OUTPUT)/boot/sldr0.bin $(OUTPUT)/boot/sldr1.bin > $@
	
_subdirs:
	cd sldr0 && make
	cd sldr1 && make

$(OUTPUT)/fs/boot/sldr.cfg:sldr.cfg
	mkdir -p $(OUTPUT)/fs/boot
	cp $< $@
