#ifndef __PROCESS_H__
#define __PROCESS_H__

#define KERNEL_CS_SELECTOR		1
#define KERNEL_DS_SELECTOR 		2 
#define TASK_SELECTOR_OFFSET 	3
#define STACK_SIZE 		(32 * 1024)

extern int _text;
extern int _etext;
extern int _data;
extern int _edata;

extern int *KERNEL_CODE_START;
extern int *KERNEL_CODE_END;
extern int *KERNEL_DATA_START;
extern int *KERNEL_DATA_END;

#define TASK_NULL		(0x00)
#define TASK_CREATING	(0x01)
#define TASK_READY		(0x02)
#define TASK_RUNNING	(0x04)
#define TASK_SLEEP 		(0x08)
#define TASK_BLOCK		(0x10)

#define BASE_RUNNING_INTERVAL 	1000
#define MAX_TASKS 				64

struct TSS32 {
	unsigned int previousTaskLink;
	unsigned int esp0;
	unsigned int ss0;
	unsigned int esp1;
	unsigned int ss1;
	unsigned int esp2;
	unsigned int ss2;
	unsigned int cr3;
	unsigned int eip;
	unsigned int eflags;
	unsigned int eax;
	unsigned int ecx;
	unsigned int edx;
	unsigned int ebx;
	unsigned int esp;
	unsigned int ebp;
	unsigned int esi;
	unsigned int edi;
	unsigned int es;
	unsigned int cs;
	unsigned int ss;
	unsigned int ds;
	unsigned int fs;
	unsigned int gs;
	unsigned int ldtr;
	unsigned int iomap;
};

struct TASK {
	int pid;
	int privilege;
	unsigned int selector;
	unsigned int flags;
	struct TSS32 tss32;
	struct TASK *next;
};

struct TASK_MANEGEMENT {
	int taskNumber;
	int runningInterval;
	struct TIMER *pTimerTask;
	struct TASK *taskHead;
	struct TASK *currentTask;
	struct TASK tasks[MAX_TASKS];
};

extern struct TASK_MANEGEMENT taskManegement;
extern struct TASK *pTaskKernel;
extern struct TASK *pTaskIdle;

void load_tr(int tr);
void farjmp(unsigned int offset, unsigned int selector);
void init_task();
int task_current();
int task_create();
void task_run(int pid);
void task_switch();
void task_sleep(int pid);
void task_wait(int pid);
void task_idle();
void task_signal(int pid);
void task_stop(int pid);

#endif // __PROCESS_H__