#include "keyboard.h"
#include "interrupt.h"
#include "string.h"
#include "libasm.h"
#include "queue.h"
#include "shell.h"

int keyboardMessageBuffer[MAX_KEYBOARD_MESSAGE];
struct QUEUE keyboardMessageQueue;

char keyboardMap[0x80] = {
	0,	0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0,   0,
	'q','w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '@', '[', 0,   0,   'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', ':', 0,   0,   ']', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0, 	0,  0,   0,   0,   0,   0,   0,   0,   0, 0, 0,  0
};

void wait_KBC_ready()
{
	while (1) {
		if ((io_in8(KBC_PORT_STATUS) & KEYBOARD_INPUT_BUFFER_FULL) == 0)
			break;
	}
}

void init_keyboard()
{
    init_queue(&keyboardMessageQueue, keyboardMessageBuffer, MAX_KEYBOARD_MESSAGE);
	wait_KBC_ready();
	io_out8(KBC_PORT_COMMAND, KEYBOARD_COMMAND_CONFIG);
	wait_KBC_ready();
	io_out8(KBC_PORT_DATA, KEYBOARD_CONFIG_BYTE);
}

void interrupt_handler_keyboard()
{
	unsigned char data;
	io_out8(PIC0_OCW2, 0x61);
	data = io_in8(KBC_PORT_DATA);
	enQueue(&keyboardMessageQueue, data);
}

