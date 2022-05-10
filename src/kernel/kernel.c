#include "kernel.h"
#include "shell.h"
#include "keyboard.h"
#include "libasm.h"
#include "interrupt.h"
#include "string.h"
#include "queue.h"
#include "memory.h"
#include "process.h"

int kernelMessageBuffer[MAX_KERNEL_MESSAGE];
struct QUEUE kernelMessageQueue;

void KernelMain()
{	
	char str[128];
	int data;
	char command[80];
	int commandLength = 0;
	struct TIMER *pTimerCursor;

	init_kernel();

	sprintf(str, "memoryManegement.memorySegmentLength: %10x\n", memoryManegement.memorySegmentLength);
	print(str);
	sprintf(str, "memoryManegement.memorySegment[0].address: %10x\n", memoryManegement.memorySegment[0].address);
	print(str);
	sprintf(str, "memoryManegement.memorySegment[0].size: %10x\n", memoryManegement.memorySegment[0].size);
	print(str);

	pTimerCursor = timer_allocate();
	timer_set_info(pTimerCursor, &kernelMessageQueue, MESSAGE_CURSOR_VISIBLE);
	timer_settime(pTimerCursor, CURSOR_INTERVAL);
	print("set cursor timer\n");

	// console
	print("\n");
	print(console.promptString);

	while (1) {
		cli();
		if (!isEmpty(&keyboardMessageQueue)) {
			sti();
			data = deQueue(&keyboardMessageQueue);
			if (data == KEY_ENTER) {  				// enter key pressed
				command[commandLength] = '\0';
				putchar(console.XPosition, console.YPosition, 0x0f, ' ');
				run_command(command, commandLength);
				print("\n");
				print(console.promptString);
				commandLength = 0;
			} else if (data == KEY_BACKSPACE) { 	// backspace key pressed
				if (commandLength > 0) {
					commandLength--;
					putchar(console.XPosition, console.YPosition, 0x0f, ' ');
					console.XPosition--;
					putchar(console.XPosition, console.YPosition, 0x0f, ' ');
				}
			} else {
				if (data >= 0x80) 
					data = 0;
				data = keyboardMap[data];
				if (data != 0) {
					command[commandLength++] = data;
					sprintf(str, "%c", (char) data);
					print(str);
				}
			}
		}

		if (!isEmpty(&kernelMessageQueue)) {
			sti();
			data = deQueue(&kernelMessageQueue);
			if (data == MESSAGE_CURSOR_VISIBLE) {
				putchar(console.XPosition, console.YPosition, 0x0f, '_');
				timer_set_info(pTimerCursor, &kernelMessageQueue, MESSAGE_CURSOR_HIDDEN);
				timer_settime(pTimerCursor, CURSOR_INTERVAL);
			} else if (data == MESSAGE_CURSOR_HIDDEN) {
				putchar(console.XPosition, console.YPosition, 0x0f, ' ');
				timer_set_info(pTimerCursor, &kernelMessageQueue, MESSAGE_CURSOR_VISIBLE);
				timer_settime(pTimerCursor, CURSOR_INTERVAL);
			} else if (data == MESSAGE_TASK_SWITCH) {
				timer_set_info(taskManegement.pTimerTask, &kernelMessageQueue, MESSAGE_TASK_SWITCH);
				timer_settime(taskManegement.pTimerTask, taskManegement.runningInterval);
				task_switch();
			}
		}
		sti();
		task_sleep(pTaskKernel->pid);
	}
}

void init_kernel()
{
	init_console();
	clear_screen();
	cli();
	print("Hello HuangOS!\n");
	print("init console\n");
	init_idt();
	print("init idt\n");
	init_queue(&kernelMessageQueue, kernelMessageBuffer, MAX_KERNEL_MESSAGE);
	print("init kernel queue\n");
	init_pic();
	print("init pic\n");
	init_pit();
	print("init pit\n");
	init_keyboard();
	print("init keyboard\n");
	init_memory();
	print("init memory\n");
	init_task();
	print("init task\n");
	sti();
	print("sti\n");
	io_out8(PIC0_IMR, 0xf8);
	io_out8(PIC1_IMR, 0xff);
}

