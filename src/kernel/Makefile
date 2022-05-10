
OBJS=head.o kernel.o libasm.o string.o descriptor.o interrupt_s.o interrupt.o keyboard.o \
		memory.o process_s.o process.o queue.o shell.o
CFLAGS=-mcmodel=32 -fno-builtin -fno-pie -fno-stack-protector -m32

all: kernel.bin
	cp ./*.bin ../../bin

kernel.bin: $(OBJS)
	ld -m elf_i386 -b elf32-i386 -o kernel -T kernel.lds -o kernel $(OBJS)
	objcopy -I elf32-i386 -S -R ".eh_frame" -R ".comment" -g -O binary kernel kernel.bin
	
head.o: head.s 
	as --32 -o $@ $^ 

libasm.o: libasm.s 
	as --32 -o $@ $^

kernel.o: kernel.h kernel.c 
	gcc $(CFLAGS)  -c $^

string.o: string.h string.c
	gcc $(CFLAGS) -c $^

descriptor.o: descriptor.h descriptor.c 
	gcc $(CFLAGS) -c $^

interrupt_s.o: interrupt.s
	as --32 -o interrupt_s.o interrupt.s 

interrupt.o: interrupt.h interrupt.c
	gcc $(CFLAGS) -c interrupt.h interrupt.c

keyboard.o: keyboard.h keyboard.c
	gcc $(CFLAGS) -c $^

memory.o: memory.h memory.c 
	gcc $(CFLAGS) -c $^

process_s.o: process.s
	as --32 -o process_s.o process.s 

process.o: process.h process.c 
	gcc $(CFLAGS) -c process.h process.c

queue.o: queue.h queue.c 
	gcc $(CFLAGS) -c $^

shell.o: shell.h shell.c 
	gcc $(CFLAGS) -c $^	

clean:
	rm -rf *.o *.gch kernel kernel.bin
