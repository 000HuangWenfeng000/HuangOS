#include <stdio.h>

int add(int a, int b);

int main()
{
    printf("char : %d\n", sizeof(char));
    printf("short : %d\n", sizeof(short));
    printf("int : %d\n", sizeof(int));
    printf("long : %d\n", sizeof(long));
    add(1, 3);

    return 0;
}

int add(int a, int b)
{
    return a + b;
}
