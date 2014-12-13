CSRC=$(shell ls *.c)
ASSRC=$(shell ls *.asm | grep -v crt0.asm)
COBJ=$(patsubst %.c,$(OUTPUT)/fs/sys/%.o,$(CSRC))
ASOBJ=$(patsubst %.asm,$(OUTPUT)/fs/sys/%.o,$(ASSRC))

$(OUTPUT)/fs/sys/kernel:$(OUTPUT)/fs/sys/crt0.o $(ASOBJ) $(COBJ)
	$(LD64) -T link.ld -o $@ $^
	rm $^

$(OUTPUT)/fs/sys/%.o:%.asm
	$(AS64) -o $@ $<

$(OUTPUT)/fs/sys/%.o:%.c
	$(CC64) -o $@ $<
