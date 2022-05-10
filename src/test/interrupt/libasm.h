#ifndef __LIBASM_H__
#define __LIBASM_H__

int io_in8(int port);
void io_out8(int port, int data);
void cli();
void sti();
void hlt();
void stihlt();
int load_eflags();
void store_eflags(int eflags);

#endif // __LIBASM_H__