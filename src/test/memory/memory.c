#include "memory.h"

struct MEMORY_MANEGEMENT memoryManegement;

void init_memory()
{
	memoryManegement.totalMemorySize = 0;
	memoryManegement.freeMemorySize = 0;
	memoryManegement.lostMemorySize = 0;
	memoryManegement.lostMemorySegment = 0;
	memoryManegement.e820Length = 0;
	memoryManegement.memorySegmentLength = 0;

	//struct E820 *p = (struct E820 *) MEMORY_STRUCT_ADDRESS;
	extern struct E820 memoryStruct[32];
	struct E820 *p = memoryStruct;
	for (int i = 0; i < MAX_E820_STRUCT_LENGTH; ++i) {
		if (p->type > 4 || p->lengthLow == 0 || p->type < 1)
			break;
		if (p->type == 1 && p->addressLow >= 0x100000) {
			memoryManegement.memorySegment[memoryManegement.memorySegmentLength].address = p->addressLow;
			memoryManegement.memorySegment[memoryManegement.memorySegmentLength].size = p->lengthLow;
			memoryManegement.memorySegmentLength++;
			memoryManegement.freeMemorySize += p->lengthLow;
		}

		memoryManegement.totalMemorySize += p->lengthLow;
		memoryManegement.e820[i].addressLow = p->addressLow;
		memoryManegement.e820[i].addressHigh = p->addressHigh;
		memoryManegement.e820[i].lengthLow = p->lengthLow;
		memoryManegement.e820[i].lengthHigh = p->lengthHigh;
		memoryManegement.e820[i].type = p->type;
		memoryManegement.e820Length++;
		p++;
	}

	memoryManegement.memorySegment[0].address 	+= KERNEL_SIZE;
	memoryManegement.memorySegment[0].size 	  	-= KERNEL_SIZE;
	memoryManegement.freeMemorySize 			-= KERNEL_SIZE;
}

void *memory_allocate(unsigned int size)
{
	if (memoryManegement.memorySegmentLength >= MAX_MEMORY_SEGMENT_LENGTH - 1) 
		return (void *) 0;

	unsigned int address;
	for (int i = 0; i < memoryManegement.memorySegmentLength; ++i) {
		if (memoryManegement.memorySegment[i].size >= size) {
			address = memoryManegement.memorySegment[i].address;
			memoryManegement.memorySegment[i].address 	+= size;
			memoryManegement.memorySegment[i].size 		-= size;
			memoryManegement.freeMemorySize				-= size;
			if (memoryManegement.memorySegment[i].size == 0) {
				memoryManegement.memorySegmentLength--;
				for (int j = i; j < memoryManegement.memorySegmentLength; ++j) {
					memoryManegement.memorySegment[i].address 	= memoryManegement.memorySegment[i+1].address;
					memoryManegement.memorySegment[i].size 		= memoryManegement.memorySegment[i+1].size;

				}
			}
			return (void *) address;
		}
	}
	return (void *) 0;
}

int memory_free(void *address, unsigned int size)
{
	int i;
	int j;
	for (i = 0; i < memoryManegement.memorySegmentLength; ++i) {
		if (memoryManegement.memorySegment[i].address > address)
			break;
	}

	if (i > 0) {
		if (memoryManegement.memorySegment[i - 1].address + memoryManegement.memorySegment[i - 1].size == address) {
			memoryManegement.memorySegment[i - 1].size += size;
			memoryManegement.freeMemorySize += size;
			if (i < memoryManegement.memorySegmentLength) {
				if (address + size  == memoryManegement.memorySegment[i].address) {
					memoryManegement.memorySegment[i - 1].size += memoryManegement.memorySegment[i].size;
					memoryManegement.memorySegmentLength--;
					for (; i < memoryManegement.memorySegmentLength; ++i) {
						memoryManegement.memorySegment[i].address = memoryManegement.memorySegment[i + 1].address;
						memoryManegement.memorySegment[i].size = memoryManegement.memorySegment[i + 1].size;
					}
				}
			}
			return 0;
		}
	}

	if (i < memoryManegement.memorySegmentLength) {
		if (address + size == memoryManegement.memorySegment[i].address) {
			memoryManegement.memorySegment[i].address = address;
			memoryManegement.memorySegment[i].size 	+= size;
			memoryManegement.freeMemorySize += size;
			return 0;
		}
	}

	if (memoryManegement.memorySegmentLength < MAX_MEMORY_SEGMENT_LENGTH) {
		for (j = memoryManegement.memorySegmentLength; j > i; --j) {
			memoryManegement.memorySegment[j].address = memoryManegement.memorySegment[j - 1].address;
			memoryManegement.memorySegment[j].size 	= memoryManegement.memorySegment[j - 1].size;
		}
		memoryManegement.memorySegmentLength++;
		memoryManegement.memorySegment[i].address = address;
		memoryManegement.memorySegment[i].size = size;
		memoryManegement.freeMemorySize += size;
		return 0;
	}

	memoryManegement.lostMemorySize += size;
	memoryManegement.lostMemorySegment++;
	return -1;
}

int memory_total()
{
	return memoryManegement.totalMemorySize;
}

int memory_total_free()
{
	return memoryManegement.freeMemorySize;
}

void *memory_allocate_4k(unsigned int size)
{
	unsigned int address;
	size = (size + 0xfff) & 0xfffff000;
	address = memory_allocate(size);
	return address;
}

int memory_free_4k(void *address, unsigned int size)
{
	int flag;
	size = (size + 0xfff) & 0xfffff000;
	flag = memory_free(address, size);
	return flag;
}

