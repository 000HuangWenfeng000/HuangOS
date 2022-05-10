#include <stdio.h>
#include "string.h"

// #define TEST(function)	function; \
// 						printf();

int main()
{
	printf("test string: \n");
	// void *memset__test(void *p, char data, int count);
	char str[32];
	memset__test(str, 'A', 31);
	str[31] = '\0';
	printf("memset__test: %s\n", str);
	
	// int is_digit__test(char ch);
	int flag;
	flag = is_digit__test('2');
	printf("is_digit__test('2') : %d \n", flag);
	flag = is_digit__test('a');
	printf("is_digit__test('a') : %d \n", flag);
	
	// int atoi__test(char *s);
	int num;
	num = atoi__test("6");
	printf("atoi__test('6') : %d\n", num);
	num = atoi__test("64");
	printf("atoi__test('64') : %d\n", num);
	num = atoi__test("-46");
	printf("atoi__test('-46') : %d\n", num);

	// void itoa__test(int num, char *buffer, int radix);
	num = 23;
	itoa__test(num, str, 10);
	printf("itoa__test(23, buf, 10) : %s\n", str);
	itoa__test(num, str, 16);
	printf("itoa__test(23, buf, 16) : %s\n", str);
	num = 233;
	itoa__test(num, str, 10);
	printf("itoa__test(233, buf, 10) : %s\n", str);
	itoa__test(num, str, 16);
	printf("itoa__test(233, buf, 16) : %s\n", str);
	num = -23;
	itoa__test(num, str, 10);
	printf("itoa__test(-23, buf, 10) : %s\n", str);
	itoa__test(num, str, 16);
	printf("itoa__test(-23, buf, 16) : %s\n", str);

	// void sprintf__test(char *string, char *format, ...);
	sprintf__test(str, "a%cc %s\n", 'B', "abcd");
	printf("sprintf__test(str, 'a%%cc %%s', 'B', 'abcd'); : %s", str);
	sprintf__test(str, "d : %d, h : %x \n", 23, 23);
	printf("sprintf__test(str, 'd : %%d, h : %%x', 23, 23); : %s", str);
	sprintf__test(str, "d : %5d, h : %5x \n", 23, 23);
	printf("sprintf__test(str, 'd : %%5d, h : %%5x', 23, 23); : %s", str);
	
	// int strlen__test(char *string);
	int len;
	len = strlen__test("123456");
	printf("strlen__test('123456') %d \n", len);
	
	// int strcmp__test(char *str1, char *str2);
	flag = strcmp__test("aa", "a");
	printf("strcmp__test('aa', 'a') %d \n", flag);
	flag = strcmp__test("bb", "bb");
	printf("strcmp__test('bb', 'bb') %d \n", flag);
	flag = strcmp__test("bba", "bbc");
	printf("strcmp__test('bba', 'bbc') %d \n", flag);

	// int strncmp__test(char *str1, char *str2, int n);
	flag = strncmp__test("aaa", "aa", 2);
	printf("strncmp__test('aaa', 'aa', 2) %d \n", flag);
	flag = strncmp__test("bb", "bb", 2);
	printf("strncmp__test('bb', 'bb') %d \n", flag);
	flag = strncmp__test("bba", "bbc", 4);
	printf("strncmp__test('bba', 'bbc', 4) %d \n", flag);
	flag = strncmp__test("mem", "mem ", 4);	
	printf("strncmp__test('mem', 'mem ', 4) %d \n", flag);

	return 0;
}
