export OUTPUT = $(abspath output)

export ASM = nasm
export CC16 = bcc -ansi -c -O
export CC32 = bcc -3 -ansi -c -O
export CPPCUR = g++ -O2 
export LD16_1 = ld86 -d 
export LD16_2 = -L/usr/lib/bcc/ -lc
export LD32_1 = ld86 -d
export LD32_2 = -L/usr/lib/bcc/i386/ -lc

all:init _boot _config _tools _vm

init:
	mkdir -p $(OUTPUT)/boot
	mkdir -p $(OUTPUT)/fs
	mkdir -p $(OUTPUT)/tools
	mkdir -p $(OUTPUT)/vm
	
	rm -f -r $(OUTPUT)/fs/*
	rm -f -r $(OUTPUT)/vm/*
	
_boot:
	cd boot && make
	
_config:
	cd config && make

_tools:
	cd tools && make

_vm:
	cd vm && make
