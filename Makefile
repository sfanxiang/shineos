export OUTPUT = $(abspath output)

export AS16 = nasm
export CC16 = bcc -ansi -c -O
export LD16_1 = ld86 -d
export LD16_2 = -L/usr/lib/bcc/ -lc
export AS64 = x86_64-unknown-linux-gnu-as
export CC64 = x86_64-unknown-linux-gnu-gcc -c -fno-builtin -fno-stack-protector -fPIC -O2
export LD64 = x86_64-unknown-linux-gnu-ld -nostdlib --oformat binary
export CPPCUR = g++ -O2

all:clean _boot _config _tools _images

clean:
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
