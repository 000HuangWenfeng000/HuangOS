#include "string.h"

void *memset__test(void *p, char data, int count)
{
	char *q = (char *) p;
	while (count--)
		*q++ = data;
	return p;
}

int is_digit__test(char ch)
{
	if ('0' <= ch && ch <= '9')
		return 1;
	return 0;
}

int atoi__test(char *s)
{
	int i = 0;
	int flag = 0;
	if (*s == '-') {
		flag = 1;
		s++;
	}
	while (is_digit__test(*s))
		i = i * 10 + (*s++) - '0';
	if (flag)
		i = -i;
	return i;
}

void itoa__test(int num, char *buffer, int radix)
{
	int i = 0;
	int flag = 0;
	char tmp;
	char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	if (num < 0 && radix == 10) {
		flag = 1;
		num = -num;
	}
	unsigned int sum = num;
	while (sum > 0) {
		buffer[i++] = digits[sum % radix];
		sum /= radix;
	}
	if (flag) 
		buffer[i++] = '-';
	for (int j = 0; j < i / 2; ++j) {
		tmp = buffer[j];
		buffer[j] = buffer[i - j - 1];
		buffer[i - j - 1] = tmp;
	}
	buffer[i] = '\0';
}

void sprintf__test(char *string, char *format, ...)
{
	int i;
	int num;
	int len;
	int index;
	char *s;
	int fieldWidth = -1;

	char buffer[128];
	char digit[32];

	va_list ap;
	va_start(ap, format);

	while (*format != '\0') {
		if (*format != '%') {
			*string++ = *format++;
			continue;
		}

		format++;

		index = 0;
		memset__test(digit , 0, sizeof(digit));
		while (is_digit__test(*format))
			digit[index++] = *format++;
		fieldWidth = atoi__test(digit);

		switch (*format) {
			case 'c':
				*string++ = (char)va_arg(ap, int);
			break;
			case 's':
				s = va_arg(ap, char *);
				for (i = 0; s[i] != '\0'; ++i)
					*string++ = s[i];
			break;
			case 'x':
			case 'X':
				memset__test(buffer, 0, sizeof(buffer));
				num = va_arg(ap, int);
				itoa__test(num, buffer, 16);
				len = strlen__test(buffer);
				len += 2;
				*string++ = '0';
				*string++ = 'x';
				if (len > fieldWidth) {
					for (i = 0; buffer[i] != '\0'; ++i)
						*string++ = buffer[i];
				} else {
					for (i = 0; i < fieldWidth - len; ++i) 
						*string++ = '0';
					for (i = 0; buffer[i] != '\0'; ++i)
						*string++ = buffer[i];
				}
			break;
			case 'd':
				memset__test(buffer , 0, sizeof(buffer));
				num = va_arg(ap, int);
				if (num < 0) {
					*string++ = '-';
					num = -num;
				}
				itoa__test(num, buffer, 10);
				len = strlen__test(buffer);
				if (len > fieldWidth) {
					for (i = 0; buffer[i] != '\0'; ++i)
						*string++ = buffer[i];
				} else {
					for (i = 0; i < fieldWidth - len; ++i) 
						*string++ = '0';
					for (i = 0; buffer[i] != '\0'; ++i)
						*string++ = buffer[i];
				}
			break;
			case '%':
				*string++ = '%';
			break;
			default:
				*string++ = '%';
				if (*format)
					*string++ = *format;
				else 
					format--;
			break;
		}
		format++;
	}

	*string = '\0';
	va_end(ap);

}

int strlen__test(char *string)
{
	int length = 0;
	while (string[length++] != '\0');
	return --length;
}

int strcmp__test(char *str1, char *str2)
{
	int i = 0;
	while (str1[i] != '\0' && str2[i] != '\0') {
		if (str1[i] != str2[i])
			return 1;
		++i;
	}

	if (str1[i] == '\0' && str2[i] == '\0')
		return 0;

	return 1;
}

int strncmp__test(char *str1, char *str2, int n)
{
	int i = 0;
	while (str1[i] != '\0' && str2[i] != '\0' && i < n) {
		if (str1[i] != str2[i])
			return 1;
		++i;
	}

	if (str1[i] == '\0' && str2[i] == '\0') 
		return 0;

	if (i == n && (str1[i] == '\0' || str2[i] == '\0'))
		return 0;

	if (i < n && str1[i] != '\0' && str2[i] != '\0')
		return 0;

	return 1;
}

