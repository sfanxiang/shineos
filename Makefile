export OUTPUT = $(abspath output)

export ASM = nasm
export CC16 = bcc -ansi -c -O
export CCCUR = gcc -O2 
export CPPCUR = g++ -O2 
export LD16_1 = ld86
export LD16_2 = -d -L/usr/lib/bcc/ -lc

all:init _boot _system _tools _vm

init:
	mkdir -p $(OUTPUT)/boot
	mkdir -p $(OUTPUT)/fs
	mkdir -p $(OUTPUT)/tools
	mkdir -p $(OUTPUT)/vm
	
	rm -f -r $(OUTPUT)/fs/*
	rm -f -r $(OUTPUT)/vm/*
	
_boot:
	cd boot && make

_system:
	cd system && make

_tools:
	cd tools && make

_vm:
	cd vm && make
