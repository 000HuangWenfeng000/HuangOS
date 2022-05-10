#ifndef __KERNEL_H__
#define __KERNEL_H__

#define MAX_KERNEL_MESSAGE				128
#define CURSOR_INTERVAL 				300

#define MESSAGE_CURSOR_HIDDEN			0
#define MESSAGE_CURSOR_VISIBLE 			1
#define MESSAGE_TASK_SWITCH				2

extern struct QUEUE kernelMessageQueue;

void init_kernel();

#endif // __KERNEL_H__


