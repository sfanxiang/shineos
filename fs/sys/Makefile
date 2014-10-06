all:_mkdir $(OUTPUT)/fs/sys/start16

_mkdir:
	mkdir -p $(OUTPUT)/fs/sys

$(OUTPUT)/fs/sys/start16:start16.c
	$(CC16) -o $(OUTPUT)/fs/sys/start16.o $<
	$(LD16_1) -o $@ $(OUTPUT)/fs/sys/start16.o $(LD16_2)
	rm $(OUTPUT)/fs/sys/start16.o
