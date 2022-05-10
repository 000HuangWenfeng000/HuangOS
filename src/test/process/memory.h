#ifndef __MEMORY_H__
#define __MEMORY_H__

#define MAX_MEMORY_SEGMENT_LENGTH 		4096
#define MEMORY_STRUCT_ADDRESS			0x7e00
#define MAX_E820_STRUCT_LENGTH			32
#define KERNEL_SIZE 					0x100000

struct E820 {
	unsigned int addressLow;
	unsigned int addressHigh;
	unsigned int lengthLow;
	unsigned int lengthHigh;
	unsigned int type;
};

struct MEMORY_SEGMENT {
	unsigned int address;
	unsigned int size;
};

struct MEMORY_MANEGEMENT {
	unsigned int freeMemorySize;
	unsigned int memorySegmentLength;
	unsigned int totalMemorySize;
	unsigned int lostMemorySize;
	unsigned int lostMemorySegment;
	unsigned int e820Length;
	struct E820 e820[MAX_MEMORY_SEGMENT_LENGTH];
	struct MEMORY_SEGMENT memorySegment[MAX_MEMORY_SEGMENT_LENGTH];
};

extern struct MEMORY_MANEGEMENT memoryManegement;

void init_memory();
void *memory_allocate(unsigned int size);
int memory_free(void *address, unsigned int size);
int memory_total();
int memory_total_free();
void *memory_allocate_4k(unsigned int size);
int memory_free_4k(void *address, unsigned int size);

#endif // __MEMORY_H__