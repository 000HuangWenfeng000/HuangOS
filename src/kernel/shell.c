#include "shell.h"
#include "queue.h"
#include "string.h"
#include "memory.h"
#include "process.h"

int consoleMessageBuffer[MAX_CONSOLE_MESSAGE];
struct QUEUE consoleMessageQueue;
struct CONSOLE console;

void init_console()
{
	console.XSize = SCREEN_WIDTH;
	console.YSize = SCREEN_HEIGHT;
	console.XPosition = 0;
	console.YPosition = 0;
	console.promptString = PROMPT_STRING;
	console.buffer = (char *) SCREEN_BUFFER_ADDRESS;
	console.pQueue = &consoleMessageQueue;
	init_queue(&consoleMessageQueue, consoleMessageBuffer, MAX_CONSOLE_MESSAGE);
	console.pQueue = &consoleMessageQueue;
}

void scroll_down()
{
	char *buffer = console.buffer;
	int width = console.XSize;
	for (int y = 0; y < console.YSize - 1; ++y) {
		for (int x = 0; x < console.XSize; ++x) {
			*(buffer + (width * y + x) * 2) 	= *(buffer + (width * (y + 1) + x) * 2);
			*(buffer + (width * y + x) * 2 + 1) = *(buffer + (width * (y + 1) + x) * 2 + 1);
		}
	}
	for (int x = 0; x < console.XSize; ++x) {
		*(buffer + (width * (console.YSize - 1) + x) * 2) = ' ';
		*(buffer + (width * (console.YSize - 1) + x) * 2 + 1) = 0x00;
	}
	console.XPosition = 0;
}

void putstr(int x, int y, char color, char *str)
{
	char *buffer = console.buffer;
	int width = console.XSize;
	int XPosition = x;
	int YPosition = y;

	for (int i = 0; str[i] != '\0'; ++i) {
		if (XPosition >= console.XSize) {
			XPosition = 0;
			YPosition++;
		}
		if (YPosition >= console.YSize) {
			YPosition = console.YSize - 1;
			scroll_down();
		}
		
		if (str[i] == '\n') {
			XPosition = 0;
			YPosition++;
			if (YPosition >= console.YSize){
				YPosition = console.YSize - 1;
				scroll_down();
			}
			continue;
		}

		*(buffer + (width * YPosition + XPosition) * 2) = str[i];
		*(buffer + (width * YPosition + XPosition) * 2 + 1) = color;
		XPosition++;
	}

	console.XPosition = XPosition;
	console.YPosition = YPosition;
}

void putchar(int x, int y, int color, char ch)
{
	*(console.buffer + (console.XSize * y + x) * 2) = ch;
	*(console.buffer + (console.XSize * y + x) * 2 + 1) = color;
}

void print(char *str)
{
	putstr(console.XPosition, console.YPosition, 0x0f, str);
}

void clear_screen()
{
	char *buffer = console.buffer;
	int width = console.XSize;
	for (int y = 0; y < console.YSize; ++y) {
		for (int x = 0; x < console.XSize; ++x) {
			*(buffer + (width * y + x) * 2) 	= ' ';
			*(buffer + (width * y + x) * 2 + 1) = 0x00;
		}
	}
	console.XPosition = 0;
	console.YPosition = 0;	
}

void run_command(char *command, int commandLength)
{
	if (strcmp(command, "cls") == 0) {
		command_clear();
	} else if (strcmp(command, "clear") == 0) {
		command_clear();
	} else if (strcmp(command, "help") == 0) {
		command_help();
	} else if (strncmp(command, "mem", 3) == 0) {
		command_mem(command, commandLength);
	} else if (strncmp(command, "task", 4) == 0) {
		command_task(command, commandLength);
	}
}

void command_clear()
{
	clear_screen();
}

void command_help()
{
	print("\n");
	print("for the reason of simplicity, the number of command is limited.\n");
	print("supported command is following : \n");
	print("    help   : show the command the os supportes\n");
	print("    clear  : clear the sreen\n");
	print("    cls    : the same as command clear\n");
	print("    mem    : show the memory info\n");
	print("        mem [info] | (0|1|...)\n");
	print("    task   : show the task info\n");
	print("        task [info] | (0|1|...)\n");
}

void command_mem(char *command, int commandLength)
{
	char parameter[80];
	char s[80];
	int parameterLength = 0;
	int i;
	parameter[0] = '\0';
	for (i = 4; i < commandLength + 1; ++i) 
		parameter[parameterLength++] = command[i];

	print("\n");
	if (parameter[0] == '\0' || strcmp(parameter, "info") == 0) {
		print("memory info : \n");
		sprintf(s, "free memory size : %dKB\n", memoryManegement.freeMemorySize / 1024);
		print(s);
		sprintf(s, "total memory size : %dKB\n", memoryManegement.totalMemorySize / 1024);
		print(s);
		return;
	}

	if (is_digit(parameter[0]) && parameterLength < 4) {
		i = atoi(parameter);
		if (i >= 0) {
			sprintf(s, "memory segment %d: \n", i);
			print(s);
			sprintf(s, "address : %10x\n", memoryManegement.memorySegment[i].address);
			print(s);
			sprintf(s, "size : %dB\n", memoryManegement.memorySegment[i].size);
			print(s);
			return;
		}
	}

	print("mem has no such parameter!\n");
}

void command_task(char *command, int commandLength)
{
	char parameter[80];
	char s[80];
	int parameterLength = 0;
	int i;
	parameter[0] = '\0';
	for (i = 5; i < commandLength + 1; ++i) 
		parameter[parameterLength++] = command[i];

	print("\n");
	if (parameter[0] == '\0' || strcmp(parameter, "info") == 0) {
		print("task info : \n");
		sprintf(s, "task number : %d\n", taskManegement.taskNumber);
		print(s);
		sprintf(s, "current task pid : %d\n", taskManegement.currentTask->pid);
		print(s);
		return;
	}

	if (is_digit(parameter[0]) && parameterLength < 4) {
		i = atoi(parameter);
		if (i >= 0) {
			sprintf(s, "task pid %d: \n", i);
			print(s);
			sprintf(s, "privilege : %d\n", taskManegement.tasks[i].privilege);
			print(s);
			sprintf(s, "flags : %d\n", taskManegement.tasks[i].flags);
			print(s);
			return;
		}
	}

	print("task has no such parameter!\n");
}



