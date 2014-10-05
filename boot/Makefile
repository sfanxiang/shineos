all:_subdirs $(OUTPUT)/boot/sldr.bin

_subdirs:
	cd sldr0 && make
	cd sldr1 && make

$(OUTPUT)/boot/sldr.bin:$(OUTPUT)/boot/sldr0.bin $(OUTPUT)/boot/sldr1.bin
	cat $(OUTPUT)/boot/sldr0.bin $(OUTPUT)/boot/sldr1.bin > $@
