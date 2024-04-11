ASM = nasm
ASMFLAGS = -f elf64 -o

%.o: %.asm
	$(ASM) $(ASMFLAGS) $@ $<
.PHONY: clean files test
clean:
	rm *.o
	rm main
files: dict.inc lib.inc colon.inc
	make lib.o main.o dict.o
	ld -o main *.o
test:
	python test.py
