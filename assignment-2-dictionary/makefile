build/%.o: src/%.asm 
	nasm -f elf64 -o $@ $<

main: build/main.o build/dict.o build/lib.o
	ld -o $@ $^


.PHONY: clean