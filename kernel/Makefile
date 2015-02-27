CHEADER=$(shell ls *.h)
CSRC=$(shell ls *.c)
ASSRC=$(shell ls *.asm | grep -v crt0.asm)
AS16SRC=$(shell ls *.as16)

COBJ=$(patsubst %.c,$(OUTPUT)/kernel/%.o,$(CSRC))
ASOBJ=$(patsubst %.asm,$(OUTPUT)/kernel/%.o,$(ASSRC))
AS16OBJ=$(patsubst %.as16,$(OUTPUT)/kernel/%.o,$(AS16SRC))

OBJ=$(OUTPUT)/kernel/crt0.o $(ASOBJ) $(COBJ) $(AS16OBJ)

$(OUTPUT)/kernel/kernel:link.ld $(OBJ)
	$(LD64) -T link.ld -o $@ $(OBJ)

$(OUTPUT)/kernel/%.o:%.asm
	$(AS64) -o $@ $<

$(OUTPUT)/kernel/%.o:%.c $(CHEADER)
	$(CC64) -o $@ $<

$(OUTPUT)/kernel/%.o:%.as16
	$(AS16) -f elf64 -o $@ $<
