#include <stdio.h>
#include "process.h"
#include "descriptor.h"

int text;
int etext;
int data;
int edata;

struct SEGMENT_DESCRIPTOR GDT_BASE_1[8192];
struct SEGMENT_DESCRIPTOR *GDT_BASE = GDT_BASE_1;
void load_tr(int tr);
void farjmp(unsigned int offset, unsigned int selector);

void printTask(int pid);
void printTasks();
void printTaskManegement();
void printGDT(int selector);
void keyboard_interrupt();

int main()
{
    printf("task : \n");
    // void init_task();
    init_task();
    printTaskManegement();
    printTasks();
    printTask(pTaskIdle->pid);
    printGDT(pTaskKernel->selector);

    // int task_current();
    printf("task_current : \n");
    int pid;
    pid = task_current();
    printTask(pid);

    // int task_create();
    printf("task_create : \n");
    int pid1;
    int pid2;
    int pid3;
    int pid4;
    int pid5;
    pid1 = task_create();
    pid2 = task_create();
    pid3 = task_create();
    pid4 = task_create();
    pid5 = task_create();
    printTaskManegement();
    printTasks();
    // printTask(pid1);
    printGDT(taskManegement.tasks[pid1].selector);    
    
    // void task_run(int pid);
    printf("task_run : \n");
    task_run(pid1);
    task_run(pid2);
    task_run(pid4);
    task_run(pid5);
    printTaskManegement();
    // printTask(pid1);
    printTasks();

    // void task_switch();
    printf("task_switch : \n");
    task_switch();
    task_switch();
    printTaskManegement();
    // printTask(pid1);
    printTasks();

    // void task_sleep(int pid);
    printf("task_sleep : \n");
    task_sleep(pid1);
    printTaskManegement();
    // printTask(pid1);
    printTasks();

    // void task_stop(int pid);
    printf("task_stop : \n");
    task_stop(pid1);
    task_stop(pid2);
    task_stop(pid4);
    printTaskManegement();
    printTask(pid1);
    printTasks();

    printf("task_sleep pidKernel : \n");
    task_sleep(pTaskKernel->pid);
    printTaskManegement();
    printTasks();

    printf("task_sleep pid5 : \n");
    task_sleep(pid5);
    printTaskManegement();
    printTask(pTaskIdle->pid);
    printTasks();

    printf("keyboard interrupt : \n");
    keyboard_interrupt();
    printTaskManegement();
    printTask(pTaskIdle->pid);
    printTasks();

    printf("task_run and task_switch : \n");
    task_run(pid5);
    task_switch();
    printTaskManegement();
    printTasks(); 

    printf("task_sleep pidKernel : \n");
    task_sleep(pTaskKernel->pid);
    printTaskManegement();
    printTask(pTaskIdle->pid);
    printTasks();

     printf("task_stop pid5: \n");
    task_stop(pid5);
    printTaskManegement();
    printTask(pid5);
    printTask(pTaskIdle->pid);
    printTasks();

    printf("keyboard interrupt : \n");
    keyboard_interrupt();
    printTaskManegement();
    printTask(pTaskIdle->pid);
    printTasks();         

    // void task_wait(int pid);
    // void task_idle();
    // void task_signal(int pid);
    

    return 0;
}

void printTaskManegement()
{
    printf("task manegement : \n");
    printf("taskManegement.taskNumber : %u \n", taskManegement.taskNumber);
    printf("taskManegement.runningInterval : %u\n", taskManegement.runningInterval);
    printf("taskManegement.taskHead : %u\n", taskManegement.taskHead);
    printf("taskManegement.currentTask : %u\n", taskManegement.currentTask);
}

void printTasks()
{
    struct TASK *pTask = taskManegement.taskHead;
    
    printf("task : \n");
    printTask(pTask->pid);
    pTask = pTask->next;
    while (pTask != taskManegement.taskHead) {
        printf("task : \n");
        printTask(pTask->pid);
        pTask = pTask->next;
    }
}

void printTask(int pid)
{
    struct TASK *pTask = taskManegement.tasks + pid;
    printf("task.pid : %u\n", pTask->pid);
    printf("task.privilege : %u\n", pTask->privilege);
    printf("task.selector : %u\n", pTask->selector);
    printf("task.flags : %u\n", pTask->flags);
    printf("task.tss32.eip : 0x%x\n", pTask->tss32.eip);
    printf("task.tss32.esp : 0x%x\n", pTask->tss32.cs);
    printf("task.next : %u\n", pTask->next);
}

void printGDT(int selector)
{
    unsigned int *p = (unsigned int *) ((char *)GDT_BASE + selector);
    printf("gdt entries at : %u\n", selector);
    printf("0x%8x 0x%08x\n", p[0], p[1]);
}

void keyboard_interrupt()
{
    pTaskIdle->flags = TASK_READY;
    pTaskKernel->flags = TASK_RUNNING;
    taskManegement.currentTask = pTaskKernel;
    farjmp(0, pTaskKernel->selector);
}

void load_tr(int tr)
{

}

void farjmp(unsigned int offset, unsigned int selector)
{

}

void hlt()
{
    
}