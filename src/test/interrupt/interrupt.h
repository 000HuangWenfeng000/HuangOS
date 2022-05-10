#ifndef INTERRUPT_H
#define INTERRUPT_H

// #define IDT_ADDRESS 


#define PIC0_ICW1		0x0020 
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2 		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR 		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3 		0x00a1
#define PIC1_ICW4		0x00a1

#define PIC0_COMMAND 	0x0020
#define PIC1_COMMAND 	0x00a0

#define PIC_EOI 		0x0020
#define IRQ_TIMER 		0x00
#define IRQ_KEYBOARD	0x01

#define MAX_TIMER		256
#define TIMER_NULL		0x00
#define TIMER_CREATING 	0x01
#define TIMER_USING		0x02
// #define TIMER_TIMEOUT 	0x04
// #define TIMER_STOPPED	0x08

#define PIT_COMMAND 	0x043
#define PIT_CHANNEL0 	0x40
#define PIT_CHANNEL1	0x41	

struct TIMER {
	unsigned int tid;
	unsigned int timeout;
	unsigned int flags;
	struct TIMER *next;
	struct QUEUE *pQueue;
	int data;
};

struct TIMER_CONTROLLER {
	unsigned int count;
	unsigned int nextTimeout;
	struct TIMER *timerHead;
	struct TIMER timers[MAX_TIMER];
};

extern struct TIMER_CONTROLLER timerController;

void init_pic(); 
void interrupt_handler21();
void interrupt_handler20();
void init_idt();
void pic_send_eoi(unsigned char irq);

void interrupt_handler_timer();
void init_pit();
struct TIMER *timer_allocate();
void timer_free(struct TIMER *pTimer);
void timer_settime(struct TIMER *pTimer, unsigned int timeout);
void timer_set_info(struct TIMER *pTimer, struct QUEUE *pQueue, int data);

#endif // INTERRUPT_H