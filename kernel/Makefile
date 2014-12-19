CHEADER=$(shell ls *.h)
CSRC=$(shell ls *.c)
ASSRC=$(shell ls *.asm | grep -v crt0.asm)
COBJ=$(patsubst %.c,$(OUTPUT)/kernel/%.o,$(CSRC))
ASOBJ=$(patsubst %.asm,$(OUTPUT)/kernel/%.o,$(ASSRC))

$(OUTPUT)/kernel/kernel:$(OUTPUT)/kernel/crt0.o $(ASOBJ) $(COBJ)
	$(LD64) -T link.ld -o $@ $^

$(OUTPUT)/kernel/%.o:%.asm
	$(AS64) -o $@ $<

$(OUTPUT)/kernel/%.o:%.c $(CHEADER)
	$(CC64) -o $@ $<
