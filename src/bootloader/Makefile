
all: boot.bin loader.bin 
	cp ./*.bin ../../bin

boot.bin: boot.asm
	nasm -f bin $^ -o $@

loader.bin: loader.asm
	nasm -f bin $^ -o $@

clean:
	rm -rf *.bin
