#include "keyboard.h"
#include "interrupt.h"
#include "string.h"
#include "libasm.h"
#include "queue.h"
#include "shell.h"

int keyboardMessageBuffere[MAX_KEYBOARD_MESSAGE];
struct QUEUE keyboardMessageQueue;

char keyTable0[0x80] = {
    0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0,   0,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '@', '[', 0,   0,   'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', ':', 0,   0,   ']', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0x5c, 0,  0,   0,   0,   0,   0,   0,   0,   0,   0x5c, 0,  0
};

// char keyTable1[0x80] = {
//     0,   0,   '!', 0x22, '#', '$', '%', '&', 0x27, '(', ')', '~', '=', '~', 0,   0,
//     'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '`', '{', 0,   0,   'A', 'S',
//     'D', 'F', 'G', 'H', 'J', 'K', 'L', '+', '*', 0,   0,   '}', 'Z', 'X', 'C', 'V',
//     'B', 'N', 'M', '<', '>', '?', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
//     0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
//     '2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//     0,   0,   0,   '_', 0,   0,   0,   0,   0,   0,   0,   0,   0,   '|', 0,   0
// };

void wait_KBC_ready()
{
	while (1) {
		if ((io_in8(KBC_PORT_STATUS) & KEYBOARD_INPUT_BUFFER_FULL) == 0)
			break;
	}
}

void init_keyboard()
{
    init_queue(&keyboardMessageQueue,keyboardMessageBuffere, MAX_KEYBOARD_MESSAGE);

	wait_KBC_ready();
	io_out8(KBC_PORT_COMMAND, KEYBOARD_COMMAND_CONFIG);
	wait_KBC_ready();
	io_out8(KBC_PORT_DATA, KEYBOARD_CONFIG_BYTE);

    // 
}

void interrupt_handler_keyboard()
{
	// io_out8(PIC0_OCW2, 0x61);
    pic_send_eoi(1);
	putstr(0, 11, 0x0f, "keyboard interrupt");
    int data = 0;
    enQueue(&keyboardMessageQueue, data);
}

