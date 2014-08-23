all:_subdir sldr.bin

sldr.bin : sldr0/sldr0.bin sldr1/sldr1.bin
	cat sldr0/sldr0.bin sldr1/sldr1.bin > $@

_subdir:
	cd sldr0 && make
	cd sldr1 && make

