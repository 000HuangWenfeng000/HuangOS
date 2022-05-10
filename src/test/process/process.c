#include "process.h"
#include "libasm.h"
#include "descriptor.h"
// #include "interrupt.h"
#include "memory.h"

int *KERNEL_CODE_START 			= &text;
int *KERNEL_CODE_END 			= &etext;
int *KERNEL_DATA_START 			= &data;
int *KERNEL_DATA_END 			= &edata;

struct TASK_MANEGEMENT taskManegement;
struct TASK *pTaskKernel;
struct TASK *pTaskIdle;
int pidKernel;
int pidIdle;

void init_task()
{
	for (int i = 0; i < MAX_TASKS; ++i) {
		set_segment_descriptor(GDT_BASE + TASK_SELECTOR_OFFSET + i, SEGMENT_TSS32_LIMIT, (int) &taskManegement.tasks[i].tss32, SEGMENT_TSS32);
		taskManegement.tasks[i].pid = i;
		taskManegement.tasks[i].selector = (TASK_SELECTOR_OFFSET + i) * 8;
		taskManegement.tasks[i].privilege = 3;
		taskManegement.tasks[i].flags = TASK_NULL;
		taskManegement.tasks[i].next = 0;
		taskManegement.tasks[i].tss32.esp = 0;
		taskManegement.tasks[i].tss32.eip = 0;
		taskManegement.tasks[i].tss32.cs = 0;
		taskManegement.tasks[i].tss32.ss = 0;
		taskManegement.tasks[i].tss32.ds = 0;
	}

	// task kernel
	pidKernel = task_create();
	pTaskKernel = taskManegement.tasks + pidKernel;
	pTaskKernel->privilege = 0;
	pTaskKernel->flags = TASK_RUNNING;
	pTaskKernel->next = pTaskKernel;

	// task idle
	pidIdle = task_create();
	pTaskIdle = taskManegement.tasks + pidIdle;
	pTaskIdle->privilege = 0;
	pTaskIdle->tss32.cs = 1 * 8;
	pTaskIdle->tss32.ss = 2 * 8;
	pTaskIdle->tss32.ds = 2 * 8;
	pTaskIdle->tss32.es = 2 * 8;
	pTaskIdle->tss32.fs = 2 * 8;
	pTaskIdle->tss32.gs = 2 * 8;
	pTaskIdle->tss32.eip = (int) &task_idle;
	pTaskIdle->flags = TASK_READY;
	//pTaskIdle->tss32.ebp = memory_allocate_4k(STACK_SIZE) + STACK_SIZE;
	//pTaskIdle->tss32.esp = memory_allocate_4k(STACK_SIZE) + STACK_SIZE;

	load_tr(pTaskKernel->selector);
	taskManegement.runningInterval = BASE_RUNNING_INTERVAL;
	taskManegement.currentTask = pTaskKernel;
	taskManegement.taskNumber = 1;
	taskManegement.taskHead = pTaskKernel;

	// taskManegement.pTimerTask = timer_allocate();
	// timer_set_info();
	// timer_settime();
}

int task_current()
{
	return taskManegement.currentTask->pid;
}

int task_create()
{
	struct TASK *pTask;
	for (int i = 0; i < MAX_TASKS; ++i) {
		if (taskManegement.tasks[i].flags == TASK_NULL) {
			pTask = &taskManegement.tasks[i];
			pTask->flags = TASK_CREATING;
			pTask->privilege = 3;
			pTask->next = 0;
			pTask->tss32.eflags = 0x00000202;
			pTask->tss32.eax = 0;
			pTask->tss32.ecx = 0;
			pTask->tss32.edx = 0;
			pTask->tss32.ebx = 0;
			pTask->tss32.ebp = 0;
			pTask->tss32.esi = 0;
			pTask->tss32.edi = 0;
			pTask->tss32.esp = 0;
			pTask->tss32.eip = 0;
			pTask->tss32.es = 0;
			pTask->tss32.cs = 0;
			pTask->tss32.ds = 0;
			pTask->tss32.ss = 0;
			pTask->tss32.fs = 0;
			pTask->tss32.gs = 0;
			pTask->tss32.iomap = 0x40000000;
			// taskManegement.taskNumber++;
			return pTask->pid;
		}
	}
	return -1;
}

void task_run(int pid)
{
	struct TASK *pTask = &taskManegement.tasks[pid];
	if (pTask->flags == TASK_NULL || pTask->flags == TASK_RUNNING || pTask->flags == TASK_READY)
		return;

	if (pTask->flags == TASK_SLEEP) {
		pTask->flags = TASK_READY;
		return;
	}

	pTask->next = taskManegement.taskHead->next;
	taskManegement.taskHead->next = pTask;
	pTask->flags = TASK_READY;
	taskManegement.taskNumber++;
}

void task_switch()
{
	if (taskManegement.currentTask == 0)
		return;

	struct TASK *pTaskCurrent = taskManegement.currentTask;
	struct TASK *pTaskNext = pTaskCurrent->next;
	
	// timer_settime();

	while (pTaskNext->flags == TASK_SLEEP)
		pTaskNext = pTaskNext->next;

	if (pTaskNext->flags == TASK_READY){
		taskManegement.currentTask = pTaskNext;
		pTaskCurrent->flags = TASK_READY;
		pTaskNext->flags = TASK_RUNNING;
		farjmp(0, pTaskNext->selector);
		return;
	}

	if (pTaskNext->flags == TASK_RUNNING) {
		return;
	}
}

void task_sleep(int pid)
{
	struct TASK *pTask = taskManegement.tasks + pid;
	struct TASK *pTaskCurrent = taskManegement.currentTask;
	struct TASK *pTaskNext = pTaskCurrent->next;
	
	if (pTask->flags == TASK_NULL || pTask->flags == TASK_CREATING || pTask->flags == TASK_SLEEP)
		return;

	if (pTask->flags == TASK_READY) {
		pTask->flags = TASK_SLEEP;
		return;
	}

	while (pTaskNext->flags == TASK_SLEEP)
		pTaskNext = pTaskNext->next;

	if (pTaskNext->flags == TASK_READY){
		taskManegement.currentTask = pTaskNext;
		pTask->flags = TASK_SLEEP;
		pTaskNext->flags = TASK_RUNNING;
		farjmp(0, pTaskNext->selector);
		return;
	}

	if (pTaskNext->flags == TASK_RUNNING) {
		pTask->flags = TASK_SLEEP;
		pTaskIdle->flags = TASK_RUNNING;
		taskManegement.currentTask = 0;
		farjmp(0, pTaskIdle->selector);
		return;
	}
}

void task_stop(int pid)
{
	struct TASK *pTask = taskManegement.tasks + pid;
	struct TASK *pTaskCurrent = taskManegement.currentTask;
	struct TASK *pTaskNext = pTaskCurrent->next;
	struct TASK *pTask1;
	if (pTask->flags == TASK_NULL)
		return;	
	if (pTask->flags == TASK_CREATING) {
		pTask->flags = TASK_NULL;
		return;
	}

	if (taskManegement.taskNumber < 2)
		return;

	if (pTask->flags == TASK_SLEEP || pTask->flags == TASK_READY) {
		pTask1 = taskManegement.taskHead;
		while (pTask1->next != pTask)
			pTask1 = pTask1->next;
		pTask1->next = pTask->next;
		pTask->flags = TASK_NULL;
		taskManegement.taskHead = pTask1;
		taskManegement.taskNumber--;
		return;
	}

	while (pTaskNext->flags == TASK_SLEEP)
		pTaskNext = pTaskNext->next;

	if (pTaskNext->flags == TASK_READY){
		pTask1 = taskManegement.taskHead;
		while (pTask1->next != pTask)
			pTask1 = pTask1->next;
		pTask1->next = pTask->next;
		pTask->flags = TASK_NULL;
		taskManegement.taskHead = pTask1;
		pTaskNext->flags = TASK_RUNNING;
		taskManegement.currentTask = pTaskNext;
		taskManegement.taskNumber--;
		farjmp(0, pTaskNext->selector);
		return;
	}

	if (pTaskNext->flags == TASK_RUNNING) {
		pTask1 = taskManegement.taskHead;
		while (pTask1->next != pTask)
			pTask1 = pTask1->next;
		pTask1->next = pTask->next;
		pTask->flags = TASK_NULL;
		taskManegement.taskHead = pTask1;
		taskManegement.taskNumber--;
		taskManegement.currentTask = 0;
		pTaskIdle->flags = TASK_RUNNING;
		farjmp(0, pTaskIdle->selector);
		return;
	}
}

void task_idle()
{
	while (1)
		hlt();
}

void task_wait(int pid)
{

}

void task_signal(int pid)
{

}
