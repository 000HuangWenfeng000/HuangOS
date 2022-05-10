#include <stdio.h>
#include "interrupt.h"
#include "queue.h"

struct GATE_DESCRIPTOR *IDT_BASE;
struct SEGMENT_DESCRIPTOR *GDT_BASE;
int buffer[32];
struct QUEUE queue;

int main()
{
    // void init_pic(); 
    // void interrupt_handler21();
    // void interrupt_handler20();
    // void init_idt();
    // void pic_send_eoi(unsigned char irq);

    printf("interrupt : \n");
    // void init_pit();
    init_pit();
    printf("init_pit : \n");
    printf("timerController.count : %u\n", timerController.count);
    printf("timerController.nextTimeout : %u \n", timerController.nextTimeout);
    struct TIMER *pTimer = &(timerController.timers[0]);
    printf("timerController.timers[0] \n");
    printf("pTimer->tid : %u \n", pTimer->tid);
    printf("pTimer->timeout : %u \n", pTimer->timeout);
    printf("pTimer->flags : %u \n", pTimer->flags);
    printf("pTimer->next : %u \n", pTimer->next);
    printf("pTimer->pQueue : %u \n", pTimer->pQueue);
    printf("pTimer->data : %d \n", pTimer->data);

    pTimer = &(timerController.timers[1]);
    printf("timerController.timers[1] \n");
    printf("pTimer->tid : %u \n", pTimer->tid);
    printf("pTimer->timeout : %u \n", pTimer->timeout);
    printf("pTimer->flags : %u \n", pTimer->flags);
    printf("pTimer->next : %u \n", pTimer->next);
    printf("pTimer->pQueue : %u \n", pTimer->pQueue);
    printf("pTimer->data : %d \n", pTimer->data);
    
    // struct TIMER *timer_allocate();
    pTimer = timer_allocate();
    printf("timer_allocate \n");
    printf("pTimer->tid : %u \n", pTimer->tid);
    printf("pTimer->timeout : %u \n", pTimer->timeout);
    printf("pTimer->flags : %u \n", pTimer->flags);
    printf("pTimer->next : %u \n", pTimer->next);
    printf("pTimer->pQueue : %u \n", pTimer->pQueue);
    printf("pTimer->data : %d \n", pTimer->data);


    // void timer_set_info(struct TIMER *pTimer, struct QUEUE *pQueue, int data);
    init_queue(&queue, buffer, 32);
    timer_set_info(pTimer, &queue, 98);
    printf("timer_set_info \n");
    printf("pTimer->tid : %u \n", pTimer->tid);
    printf("pTimer->timeout : %u \n", pTimer->timeout);
    printf("pTimer->flags : %u \n", pTimer->flags);
    printf("pTimer->next : %u \n", pTimer->next);
    printf("pTimer->pQueue : %u \n", pTimer->pQueue);
    printf("pTimer->data : %d \n", pTimer->data);

    // void timer_settime(struct TIMER *pTimer, unsigned int timeout);
    timer_settime(pTimer, 10);
    printf("timer_settime \n");
    printf("pTimer->tid : %u \n", pTimer->tid);
    printf("pTimer->timeout : %u \n", pTimer->timeout);
    printf("pTimer->flags : %u \n", pTimer->flags);
    printf("pTimer->next : %u \n", pTimer->next);
    printf("pTimer->pQueue : %u \n", pTimer->pQueue);
    printf("pTimer->data : %d \n", pTimer->data); 

    pTimer = timer_allocate();
    timer_set_info(pTimer, &queue, 98);
    timer_settime(pTimer, 20);

    pTimer = timer_allocate();
    struct TIMER *pTimer1;
    pTimer1 = pTimer;
    timer_set_info(pTimer, &queue, 98);
    timer_settime(pTimer, 30);  

    pTimer = timer_allocate();
    struct TIMER *pTimer2;
    pTimer2 = pTimer;
    timer_set_info(pTimer, &queue, 98);
    timer_settime(pTimer, 5);        

    pTimer = timerController.timerHead;
    while (1) {
        printf("pTimer->next \n");
        printf("pTimer->tid : %u \n", pTimer->tid);
        printf("pTimer->timeout : %u \n", pTimer->timeout);
        printf("pTimer->flags : %u \n", pTimer->flags);
        printf("pTimer->next : %u \n", pTimer->next);
        printf("pTimer->pQueue : %u \n", pTimer->pQueue);
        printf("pTimer->data : %d \n", pTimer->data); 
        if (pTimer->next == 0)
            break;
        pTimer = pTimer->next;   
    }

    // void timer_free(struct TIMER *pTimer);
    timer_free(pTimer1);
    timer_free(pTimer2);
    printf("timer_free\n");
    pTimer = timerController.timerHead;
    while (1) {
        printf("pTimer->next \n");
        printf("pTimer->tid : %u \n", pTimer->tid);
        printf("pTimer->timeout : %u \n", pTimer->timeout);
        printf("pTimer->flags : %u \n", pTimer->flags);
        printf("pTimer->next : %u \n", pTimer->next);
        printf("pTimer->pQueue : %u \n", pTimer->pQueue);
        printf("pTimer->data : %d \n", pTimer->data); 
        if (pTimer->next == 0)
            break;
        pTimer = pTimer->next;   
    }    

    // void intterupt_handler_timer();
    for (int i = 0; i < 22; ++i) 
        interrupt_handler_timer();
    printf("interrupt_handler_timer : \n");
    pTimer = timerController.timerHead;
    while (1) {
        printf("pTimer->next \n");
        printf("pTimer->tid : %u \n", pTimer->tid);
        printf("pTimer->timeout : %u \n", pTimer->timeout);
        printf("pTimer->flags : %u \n", pTimer->flags);
        printf("pTimer->next : %u \n", pTimer->next);
        printf("pTimer->pQueue : %u \n", pTimer->pQueue);
        printf("pTimer->data : %d \n", pTimer->data); 
        if (pTimer->next == 0)
            break;
        pTimer = pTimer->next;   
    }
    int data;
    while (!isEmpty(&queue)) {
        data = deQueue(&queue);
        printf("deQueue : %d\n", data);
    }

    return 0;
}

void io_out8(int , int )
{

}

int io_in8(int)
{

}

void interrupt_handler20()
{

}

void interrupt_handler21()
{

}

void cli()
{

}

void sti()
{

}

int load_eflags()
{

}

void store_eflags()
{

}

void putstr(int x, int y, char c, char *s)
{

}
