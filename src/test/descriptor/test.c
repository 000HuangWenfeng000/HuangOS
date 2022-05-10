#include <stdio.h>
#include "descriptor.h"

struct GATE_DESCRIPTOR *IDT_BASE;
struct SEGMENT_DESCRIPTOR *GDT_BASE;

struct GATE_DESCRIPTOR IDT_BASE_1[8192];
struct SEGMENT_DESCRIPTOR GDT_BASE_1[255];

void init_gdt();
void set_segment_descriptor(struct SEGMENT_DESCRIPTOR *pDescriptor, int limit, int base, int accessRight);
void set_gate_descriptor(struct GATE_DESCRIPTOR *pDescriptor, int offset, int selector, int accessRight);
void interrupt_handler20();
void interrupt_handler21();


int main()
{
    IDT_BASE = IDT_BASE_1;
    GDT_BASE = GDT_BASE_1;
    // void init_gdt();
    // void set_segment_descriptor(struct SEGMENT_DESCRIPTOR *pDescriptor, int limit, int base, int accessRight);
    set_segment_descriptor(GDT_BASE + 1, 0xffffffff, 0x00000, SEGMENT_CODE32_ER);
    set_segment_descriptor(GDT_BASE + 2, 0xffffffff, 0x00000, SEGMENT_DATA32_RW);
    printf("set_segment_descriptor \n");
    unsigned int data;
    data = GDT_BASE[1].limitLow;
    printf("limitLow : %x\n", data);
    data = GDT_BASE[1].baseLow;
    printf("baseLow : %x\n", data);
    data = GDT_BASE[1].baseMiddle;
    printf("baseMiddle : %x\n", data);
    data = GDT_BASE[1].accessRight;
    printf("accessRight : %x\n", data);
    data = GDT_BASE[1].limitHigh;
    printf("limitHigh : %x\n", data);
    data = GDT_BASE[1].baseHigh;
    printf("baseHigh : %x\n", data);
    int *p;
    p = (int *) (GDT_BASE + 1);
    printf("0x%08x ", p[0]);
    printf("0x%08x \n", p[1]);

    // void set_gate_descriptor(struct GATE_DESCRIPTOR *pDescriptor, int offset, int selector, int accessRight);    
    set_gate_descriptor(IDT_BASE + 0x0020, (int) interrupt_handler20 , 1 * 8, INTERRUPT_GATE_ACCESS);
    set_gate_descriptor(IDT_BASE + 0x0021, (int) interrupt_handler21 , 1 * 8, INTERRUPT_GATE_ACCESS);
    printf("set_segment_descriptor \n");
    data = IDT_BASE[0x20].offsetLow;
    printf("offsetLow : %x\n", data);
    data = IDT_BASE[0x20].selector;
    printf("selector : %x\n", data);
    data = IDT_BASE[0x20].count;
    printf("count : %x\n", data);
    data = IDT_BASE[0X20].accessRight;
    printf("accessRight : %x\n", data);
    data = IDT_BASE[0x20].offsetHigh;
    printf("offsetHigh : %x\n", data);
    p = (int *) (IDT_BASE + 0x20);
    printf("0x%08x ", p[0]);
    printf("0x%08x \n", p[1]);
    return 0;
}

void interrupt_handler20()
{
    printf("interrupt_handler20");
}
void interrupt_handler21()
{
    printf("interrupt_handler21");
}
