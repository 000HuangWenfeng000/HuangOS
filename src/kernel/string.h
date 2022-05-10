
#ifndef __STRING_H__
#define __STRING_H__

typedef char *va_list;
#define va_start(ap, p) 	(ap = (char *) (&(p) + 1))
#define va_arg(ap, type)	((type *) (ap += sizeof(type)))[-1]
#define va_end(ap)			

void *memset(void *p, char data, int count);
int is_digit(char ch);
int atoi(char *s);
void itoa(int num, char *buffer, int radix);
void sprintf(char *string, char *format, ...);
int strlen(char *string);
int strcmp(char *str1, char *str2);
int strncmp(char *str1, char *str2, int n);

#endif // __STRING_H__