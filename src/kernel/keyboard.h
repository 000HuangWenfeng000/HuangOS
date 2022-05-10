#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#define KBC_PORT_DATA					0x60
#define KBC_PORT_STATUS 				0x64
#define KBC_PORT_COMMAND 				0x64

#define KEYBOARD_INPUT_BUFFER_FULL	 	0x02
#define KEYBOARD_COMMAND_CONFIG 		0x60
#define KEYBOARD_CONFIG_BYTE 			0x47
#define MAX_KEYBOARD_MESSAGE 			128

#define KEY_ENTER						0x1c
#define KEY_BACKSPACE					0x0e

extern char keyboardMap[0x80];
extern struct QUEUE keyboardMessageQueue;

void wait_KBC_ready();
void init_keyboard();
void interrupt_handler_keyboard();

#endif // __KEYBOARD_H__