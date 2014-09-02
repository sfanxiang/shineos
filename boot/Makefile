all:$(FS)/boot/sldr.cfg _subdirs sldr.bin

sldr.bin:sldr0/sldr0.bin sldr1/sldr1.bin
	cat sldr0/sldr0.bin sldr1/sldr1.bin > $@
	
_subdirs:
	cd sldr0 && make
	cd sldr1 && make

$(FS)/boot/sldr.cfg:sldr.cfg
	mkdir -p $(FS)/boot
	cp $< $@