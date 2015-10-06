export OUTPUT = $(abspath output)

export AS16 = nasm
export CC16 = bcc -ansi -c -O
export LD16_1 = ld86 -d
export LD16_2 = -L/usr/lib/bcc/ -lc
export AS64 = x86_64-unknown-linux-gnu-gcc -c -ffreestanding
export CC64 = x86_64-unknown-linux-gnu-gcc -c -ffreestanding -fno-stack-protector -fPIC -fvisibility=hidden -mno-mmx -mno-red-zone -mno-sse -mno-sse2 -O2
export LD64 = x86_64-unknown-linux-gnu-gcc -nostdlib -Wl,--oformat=binary
export CXXCUR = g++ -O2

all:_clean _boot _config _kernel _tools _fs _images

_clean:
	mkdir -p $(OUTPUT)/boot
	mkdir -p $(OUTPUT)/fs
	mkdir -p $(OUTPUT)/kernel
	mkdir -p $(OUTPUT)/tools
	mkdir -p $(OUTPUT)/images
	
	rm -f -r $(OUTPUT)/fs/*
	rm -f -r $(OUTPUT)/images/*
	
_boot:
	cd boot && make
	
_config:
	cd config && make

_kernel:
	cd kernel && make

_tools:
	cd tools && make

_fs:
	mkdir -p $(OUTPUT)/fs/sys
	cp $(OUTPUT)/boot/start $(OUTPUT)/fs/sys/start
	cp $(OUTPUT)/kernel/kernel $(OUTPUT)/fs/sys/kernel

_images:
	cd images && make

clean:
	rm -f -r $(OUTPUT)
