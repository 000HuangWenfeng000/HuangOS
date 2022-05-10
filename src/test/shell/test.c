#include <stdio.h>
#include "shell.h"

void print_buffer(char *buffer);

int main()
{
    // void scroll_down();
    char buffer[80 * 25 * 2];
    for (int i = 0; i < 80 * 25 * 2; ++i)
        buffer[i] = '.';
    init_console();
    console.buffer = buffer;
    print_buffer(buffer);
    
    // void putstr(int x, int y, char color, char *str);
    printf("putstr \n");
    putstr(0, 0, 0x0f, "hello HuangOS\n");
    print_buffer(buffer);
    
    // void print(char *str);
    printf("print \n");
    print("hello \nworld");
    print_buffer(buffer);

    // void scroll_down();
    printf("scrolldown\n");
    scroll_down();
    print_buffer(buffer);

    // void putstr(int x, int y, char color, char *str);
    printf("putstr at last line \n");
    putstr(78, 24, 0x0f, "line line line\nline line\nline");
    print_buffer(buffer);

    // void clear_screen();
    printf("clear_screen()");
    clear_screen();
    print_buffer(buffer);
    
    return 0;
}

void print_buffer(char *buffer)
{
    for (int y = 0; y < 25; ++y) {
           for (int x = 0; x < 80; ++x)
            printf("%c", buffer[(y * 80 + x) * 2]);
        printf("\n");
    }
}

