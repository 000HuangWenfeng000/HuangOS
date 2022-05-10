#include "interrupt.h"
#include "libasm.h"
#include "descriptor.h"
#include "keyboard.h"
#include "shell.h"
#include "queue.h"
#include "string.h"

struct TIMER_CONTROLLER timerController;

void init_pic()
{
	io_out8(PIC0_IMR, 0xff);
	io_out8(PIC1_IMR, 0xff);

	io_out8(PIC0_ICW1, 0x11);
	io_out8(PIC0_ICW2, 0x20);
	io_out8(PIC0_ICW3, 0x04); 
	io_out8(PIC0_ICW4, 0x01);

	io_out8(PIC1_ICW1, 0x11);
	io_out8(PIC1_ICW2, 0x28);
	io_out8(PIC1_ICW3, 0x02);
	io_out8(PIC1_ICW4, 0x01);

	io_out8(PIC0_IMR, 0xfb);
	io_out8(PIC1_IMR, 0xff);
}

void pic_send_eoi(unsigned char irq)
{
	if (irq >= 8)
		io_out8(PIC0_COMMAND, PIC_EOI);
	io_out8(PIC1_COMMAND, PIC_EOI);
}

void init_idt()
{
	set_gate_descriptor(IDT_BASE + 0x00, (int) interrupt_handler00, 1 * 8, INTERRUPT_GATE32_ACCESS);
	set_gate_descriptor(IDT_BASE + 0x01, (int) interrupt_handler01, 1 * 8, INTERRUPT_GATE32_ACCESS);
	set_gate_descriptor(IDT_BASE + 0x06, (int) interrupt_handler06, 1 * 8, INTERRUPT_GATE32_ACCESS);
	set_gate_descriptor(IDT_BASE + 0x08, (int) interrupt_handler08, 1 * 8, INTERRUPT_GATE32_ACCESS);
	set_gate_descriptor(IDT_BASE + 0x0d, (int) interrupt_handler0d, 1 * 8, INTERRUPT_GATE32_ACCESS);
	set_gate_descriptor(IDT_BASE + 0x20, (int) interrupt_handler20, 1 * 8, INTERRUPT_GATE32_ACCESS);
	set_gate_descriptor(IDT_BASE + 0x21, (int) interrupt_handler21, 1 * 8, INTERRUPT_GATE32_ACCESS);
	set_gate_descriptor(IDT_BASE + 0x27, (int) interrupt_handler27, 1 * 8, INTERRUPT_GATE32_ACCESS);
}

void init_pit()
{
	io_out8(PIT_COMMAND, 0x34);
	io_out8(PIT_CHANNEL0, TIMER_DIV(1000) % 255);
	io_out8(PIT_CHANNEL0, TIMER_DIV(1000) / 255);

	timerController.count = 0;
	timerController.nextTimeout = 0xffffffff;
	for (int i = 0; i < MAX_TIMER; ++i) {
		timerController.timers[i].tid = i;
		timerController.timers[i].flags = TIMER_NULL;
		timerController.timers[i].next = (void *) 0;
		timerController.timers[i].timeout = 0;
		timerController.timers[i].pQueue = 0;
		timerController.timers[i].data = -1;
	}

	struct TIMER *pTimer = timer_allocate();
	pTimer->timeout = 0xffffffff;
	pTimer->flags = TIMER_USING;
	pTimer->next = 0;
	timerController.timerHead = pTimer;
	timerController.nextTimeout = 0xffffffff;
}

void interrupt_handler_timer()
{
	io_out8(PIC0_OCW2, 0x60);
	timerController.count++;
	char s[32];
	if (timerController.nextTimeout > timerController.count)
		return;
	struct TIMER *pTimer = timerController.timerHead;
	while (1) {
		if (pTimer->timeout > timerController.count) 
			break;

		enQueue(pTimer->pQueue, pTimer->data);
		pTimer->flags = TIMER_NULL;
		pTimer = pTimer->next;
	}

	timerController.timerHead = pTimer;
	timerController.nextTimeout = pTimer->timeout;
}

void interrupt_handler_DE()
{
	print("interrupt : DE\n");
}

void interrupt_handler_DB()
{
	print("interrupt : DB\n");
}

void interrupt_handler_DF()
{
	print("interrupt : FE\n");
}

void interrupt_handler_UD()
{
	print("interrupt : UD\n");
}

void interrupt_handler_PF()
{
	print("interrupt : PF\n");
}

void interrupt_handler_irq7()
{
	print("interrupt : irq7\n");
}


struct TIMER *timer_allocate()
{
	for (int i = 0; i < MAX_TIMER; ++i) {
		if (timerController.timers[i].flags == TIMER_NULL) {
			timerController.timers[i].flags = TIMER_CREATING;
			return &timerController.timers[i];
		}
	}
	return (void *) 0;
}

void timer_free(struct TIMER *pTimer)
{
	struct TIMER *pTimer1;
	struct TIMER *pTimer2;

	pTimer1 = timerController.timerHead;
	if (pTimer == pTimer1) {
		timerController.timerHead = pTimer->next;
		pTimer->flags = TIMER_NULL;
		return;
	}
	
	while (1) {
		if (pTimer1->next == 0)
			break;

		pTimer2 = pTimer1;
		pTimer1 = pTimer1->next;
		if (pTimer == pTimer1) {
			pTimer2->next = pTimer1->next;
			pTimer->flags = TIMER_NULL;
			return;
		}
	}
}

void timer_settime(struct TIMER *pTimer, unsigned int timeout)
{
	int eflags;
	struct TIMER *pTimer1;
	struct TIMER *pTimer2;
	pTimer->timeout = timeout + timerController.count;
	pTimer->flags = TIMER_USING;
	eflags = load_eflags();
	cli();

	pTimer1 = timerController.timerHead;
	if (pTimer->timeout <= pTimer1->timeout) {
		timerController.timerHead = pTimer;
		pTimer->next = pTimer1;
		timerController.nextTimeout = pTimer->timeout;
		store_eflags(eflags);
		return ;
	}

	while (1) {
		pTimer2 = pTimer1;
		pTimer1 = pTimer1->next;
		if (pTimer->timeout < pTimer1->timeout) {
			pTimer2->next = pTimer;
			pTimer->next = pTimer1;
			store_eflags(eflags);
			return;
		}
	}
}

void timer_set_info(struct TIMER *pTimer, struct QUEUE *pQueue, int data)
{
	pTimer->data = data;
	pTimer->pQueue = pQueue;
}
