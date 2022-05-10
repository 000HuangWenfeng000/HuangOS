
#ifndef STRING_H
#define STRING_H

typedef char *va_list;
#define va_start(ap, p) 	(ap = (char *) (&(p) + 1))
#define va_arg(ap, type)	((type *) (ap += sizeof(type)))[-1]
#define va_end(ap)			

void *memset__test(void *p, char data, int count);
int is_digit__test(char ch);
int atoi__test(char *s);
void itoa__test(int num, char *buffer, int radix);
void sprintf__test(char *string, char *format, ...);
int strlen__test(char *string);
int strcmp__test(char *str1, char *str2);
int strncmp__test(char *str1, char *str2, int n);

#endif // STRING_H