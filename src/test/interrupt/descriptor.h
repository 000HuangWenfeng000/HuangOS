#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

#define INTERRUPT_GATE_ACCESS 	0x008e
#define SEGMENT_DATA32_RW		0x4092
#define SEGMENT_CODE32_ER 		0x409a

struct SEGMENT_DESCRIPTOR {
	short limitLow;
	short baseLow;
	char baseMiddle;
	char accessRight;
	char limitHigh;
	char baseHigh;
};
struct GATE_DESCRIPTOR {
	short offsetLow;
	short selector;
	char count;
	char accessRight;
	short offsetHigh;
};

extern struct GATE_DESCRIPTOR *IDT_BASE;
extern struct SEGMENT_DESCRIPTOR *GDT_BASE;

void init_gdt();
void set_segment_descriptor(struct SEGMENT_DESCRIPTOR *pDescriptor, int limit, int base, int accessRight);
void set_gate_descriptor(struct GATE_DESCRIPTOR *pDescriptor, int offset, int selector, int accessRight);

#endif // __DESCRIPTOR_H__