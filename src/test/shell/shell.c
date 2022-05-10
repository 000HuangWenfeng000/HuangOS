#include "shell.h"
#include "queue.h"

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