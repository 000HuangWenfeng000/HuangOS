#ifndef __SHELL_H__
#define __SHELL_H__

#define SCREEN_BUFFER_ADDRESS 			0xb8000
#define MAX_CONSOLE_MESSAGE 			128
#define SCREEN_WIDTH 					80
#define SCREEN_HEIGHT					25
#define PROMPT_STRING					"HWF@HuangOS # "

struct CONSOLE {
	int XSize;
	int YSize;
	int XPosition;
	int YPosition;
	char *buffer;
	char *promptString;
	struct QUEUE *pQueue;
};

extern struct CONSOLE console;
extern int consoleMessageBuffer[MAX_CONSOLE_MESSAGE];
extern struct QUEUE consoleMessageQueue;

void scroll_down();
void scroll_up();
void putstr(int x, int y, char color, char *str);
void print(char *str);
void clear_screen();
void init_console();
void run_cmd(char *command);
void cmd_clear();
void cmd_mem();
// void print();

#endif // __SHELL_H__