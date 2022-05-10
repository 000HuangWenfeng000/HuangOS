#include "descriptor.h"

void set_segment_descriptor(struct SEGMENT_DESCRIPTOR *pDescriptor, int limit, int base, int accessRight)
{
	pDescriptor->limitLow 		= limit & 0xffff;
	pDescriptor->baseLow 		= base & 0xffff;
	pDescriptor->baseMiddle 	= (base >> 16) & 0xff;
	pDescriptor->accessRight 	= accessRight & 0xff;
	pDescriptor->limitHigh 		= ((limit >> 16) & 0x0f) | ((accessRight >> 8) & 0xf0);
	pDescriptor->baseHigh		= (base >> 24) & 0xff;
}

void set_gate_descriptor(struct GATE_DESCRIPTOR *pDescriptor, int offset, int selector, int accessRight)
{
	pDescriptor->offsetLow 		= offset & 0xffff;
	pDescriptor->selector 		= selector;
	pDescriptor->count 			= (accessRight >> 8) & 0xff;
	pDescriptor->accessRight 	= accessRight & 0xff;
	pDescriptor->offsetHigh	 	= (offset >> 16) & 0xffff;
}


void init_gdt()
{

}
