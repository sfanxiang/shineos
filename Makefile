export OUTPUT = $(abspath output)

export ASM = nasm
export CC16 = bcc -ansi -c -O
export CC64 = x86_64-tcc -nostdlib -Wl,--oformat=binary 
export CC64_STRIP = dd iflag=skip_bytes skip=96 
export CPPCUR = g++ -O2 
export LD16_1 = ld86 -d 
export LD16_2 = -L/usr/lib/bcc/ -lc

all:init _boot _config _tools _images

init:
	mkdir -p $(OUTPUT)/boot
	mkdir -p $(OUTPUT)/fs
	mkdir -p $(OUTPUT)/tools
	mkdir -p $(OUTPUT)/images
	
	rm -f -r $(OUTPUT)/fs/*
	rm -f -r $(OUTPUT)/images/*
	
_boot:
	cd boot && make
	
_config:
	cd config && make

_tools:
	cd tools && make

_images:
	cd images && make
