#include <stdio.h>
#include "memory.h"

struct E820 memoryStruct[32];

void preparation();
void printMemoryManegement();
void printMemorySegment();
void printE820();

int main()
{
    preparation();
    printf("memory : \n");
    // void init_memory();
    init_memory();
    printf("init_memory : \n");
    printMemoryManegement();
    printE820();
    printMemorySegment();
    
    // void *memory_allocate(unsigned int size);
    void *address;
    unsigned int size = 64 * 1024;
    void *address1;
    unsigned int size1 = 8 * 1024;
    void *address2;
    unsigned int size2 = 763;
    void *address3;
    unsigned int size3 = 32 * 1024;
    void *address4;
    unsigned int size4 = 64 * 1024; 
    address = memory_allocate(size);
    address1 = memory_allocate(size1);
    address2 = memory_allocate(size2);
    address3 = memory_allocate(size3);
    address4 = memory_allocate(size4);
    printf("memory_allocate : size : %u address : 0x%x\n", size, address);
    printf("memory_allocate : size1 : %u address1 : 0x%x\n", size1, address1);
    printf("memory_allocate : size2 : %u address2 : 0x%x\n", size2, address2);
    printf("memory_allocate : size3 : %u address3 : 0x%x\n", size3, address3);
    printf("memory_allocate : size4 : %u address4 : 0x%x\n", size4, address4);
    printMemoryManegement();
    printMemorySegment();

    // int memory_free(void *address, unsigned int size);
    memory_free(address, size);
    printf("memory_free : address : 0x%x size : %u\n", address, size);
    printMemoryManegement();
    printMemorySegment();

    memory_free(address3, size3);
    printf("memory_free : address3 : 0x%x size3 : %u\n", address3, size3);
    printMemoryManegement();
    printMemorySegment();

    memory_free(address4, size4);
    printf("memory_free : address4 : 0x%x size4 : %u\n", address4, size4);
    printMemoryManegement();
    printMemorySegment();

    void *address5;
    unsigned int size5 = 16 * 1024;
    address5 = memory_allocate(size5);
    printf("memory_allocate : size5 : %u address5 : 0x%x\n", size5, address5);
    printMemoryManegement();
    printMemorySegment();

    memory_free(address2, size2);
    printf("memory_free : address2 : 0x%x size2 : %u\n", address2, size2);
    printMemoryManegement();
    printMemorySegment(); 

    memory_free(address1, size1);
    printf("memory_free : address1 : 0x%x size1 : %u\n", address1, size1);
    printMemoryManegement();
    printMemorySegment();

    memory_free(address5, size5);
    printf("memory_free : address5 : 0x%x size5 : %u\n", address5, size5);
    printMemoryManegement();
    printMemorySegment();     

    // int memory_total();
    int totalMemorySize;
    totalMemorySize = memory_total();
    printf("memory_total : %u \n", totalMemorySize);
    
    // int memory_total_free();
    int freeMemorySize;
    freeMemorySize = memory_total_free();
    printf("memory_total_free : %u \n", freeMemorySize);
    // void *memory_allocate_4k(unsigned int size);
    void *address6;
    unsigned int size6 = 745;
    address6 = memory_allocate_4k(size6);
    printf("memory_allocate_4k : size6 : %u address6 : 0x%x\n", size6, address6);
    printMemoryManegement();
    printMemorySegment();  

    // int memory_free_4k(void *address, unsigned int size);
    memory_free_4k(address6, size6);
    printf("memory_free_4k : address6 : 0x%x size6 : %u\n", address6, size6);
    printMemoryManegement();
    printMemorySegment();

    return 0;
}

void preparation()
{
    memoryStruct[0].addressLow  = 0;
    memoryStruct[0].addressHigh = 0;
    memoryStruct[0].lengthLow   = 0x9f000;
    memoryStruct[0].lengthHigh  = 0;
    memoryStruct[0].type        = 1;

    memoryStruct[1].addressLow  = 0x9f000;
    memoryStruct[1].addressHigh = 0;
    memoryStruct[1].lengthLow   = 0x1000;
    memoryStruct[1].lengthHigh  = 0;
    memoryStruct[1].type        = 2;

    memoryStruct[2].addressLow  = 0xe8000;
    memoryStruct[2].addressHigh = 0;
    memoryStruct[2].lengthLow   = 0x18000;
    memoryStruct[2].lengthHigh  = 0;
    memoryStruct[2].type        = 2;

    memoryStruct[3].addressLow  = 0x100000;
    memoryStruct[3].addressHigh = 0;
    memoryStruct[3].lengthLow   = 0x1ef0000;
    memoryStruct[3].lengthHigh  = 0;
    memoryStruct[3].type        = 1;

    memoryStruct[4].addressLow  = 0x01ff0000;
    memoryStruct[4].addressHigh = 0;
    memoryStruct[4].lengthLow   = 0x00010000;
    memoryStruct[4].lengthHigh  = 0;
    memoryStruct[4].type        = 3;

    memoryStruct[4].addressLow  = 0xfffc0000;
    memoryStruct[4].addressHigh = 0;
    memoryStruct[4].lengthLow   = 0x00040000;
    memoryStruct[4].lengthHigh  = 0;
    memoryStruct[4].type        = 2;
}

void printMemoryManegement()
{
    printf("memoryManegement.freeMemorySize : %u\n", memoryManegement.freeMemorySize);
    printf("memoryManegement.memorySegmentLength : %d\n", memoryManegement.memorySegmentLength);
    printf("memoryManegement.totalMemorySize : %d\n", memoryManegement.totalMemorySize);
    printf("memoryManegement.lostMemorySize : %d\n", memoryManegement.lostMemorySize);
    printf("memoryManegement.lostMemorySegment : %d\n", memoryManegement.lostMemorySegment);
    printf("memoryManegement.e820Length : %d\n", memoryManegement.e820Length);
}

void printE820()
{
    printf("e820 : \n");
    struct E820 *p = memoryManegement.e820;
    for (int i = 0; i < MAX_E820_STRUCT_LENGTH; ++i) {
        if (p->type > 4 || p->lengthLow == 0 || p->type < 1)
            break;
        printf("memoryManegement.e820[%d].addressLow : 0x%x\n", i, p->addressLow);
        printf("memoryManegement.e820[%d].addressHigh : 0x%x\n", i, p->addressHigh);
        printf("memoryManegement.e820[%d].lengthLow : 0x%x\n", i, p->lengthLow);
        printf("memoryManegement.e820[%d].lengthHigh : 0x%x\n", i, p->lengthHigh);
        printf("memoryManegement.e820[%d].type : 0x%d\n", i, p->type);
        p++;
    }
}

void printMemorySegment()
{
    printf("memorySegment: \n");
    unsigned int address;
    for (int i = 0; i < memoryManegement.memorySegmentLength; ++i) {
        printf("memoryManegement.memorySegment[%d].address : 0x%x\n", i, memoryManegement.memorySegment[i].address);
        printf("memoryManegement.memorySegment[%d].size : 0x%x\n", i, memoryManegement.memorySegment[i].size);
    }
}