/*
 *  linux/lib/vsprintf.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

#include <stdarg.h> 
/*  %[标志][输出最小宽度][.精度][长度]格式字符 */
#undef PF_FLAGS   //[标志]: + - ' ' #
#undef PF_WPL  /*最小宽度，精度，长度*/
#undef PF_XFMT  /* %n %o %p %g %e*/

#define SIGN	2		/* unsigned/signed long */
#define LARGE	64		/* use 'ABCDEF' instead of 'abcdef' */

#define size_t char

char todigits(char n)
{
   if(n<10)
     return '0'+n;
   else
     return 'a'+n-15;
}
char * number(char * str, long num, char base, char size,char type)
{
	char c,sign,tmp[6];
	char i;
	if (base < 2 || base > 16)
		return 0;
	c =' ';
	sign = 0;
	if (type & SIGN) {
		if (num < 0) {
			sign = '-';
			num = -num;
			size--;
		} 
	}
	i = 0;
	if (num == 0)
		tmp[i++]='0';
	else while (num != 0) {
	    int xx;
		xx = num;
		num=xx/base;
		tmp[i++] = todigits(xx%base);
	}

	while(size-->0)
			*str++ = ' ';
	if (sign)
		*str++ = sign;
	while (size-- > 0)
			*str++ = c;
	while (i-- > 0)
		*str++ = tmp[i];
	while (size-- > 0)
		*str++ = ' ';
	return str;
}




#define isdigit(c)	( (c>='0' && c<= '9') || ( c>='a' && c<= 'f') || ( c>='A' && c<= 'F')  )
#define islower(c)	( c>='a' && c<= 'z' )
#define isupper(c)	( c>='A' && c<= 'Z' ) 


unsigned char __tolower(unsigned char c)
{
	if (isupper(c))
		c -= 'A'-'a';
	return c;
}

unsigned char __toupper(unsigned char c)
{
	if (islower(c))
		c -= 'a'-'A';
	return c;
}

#define tolower(c) __tolower(c)
#define toupper(c) __toupper(c)

long atoi(register char *p);

 
void * d_memchr (void const *s, int c, size_t n)
{
  unsigned char const *p = s;
  unsigned char const *lim = p + n;

  for (;  p < lim;  p++)
    if (*p == c)
      return p;
  return 0;
}


size_t	strnlen (const char *string, size_t maxlen)
{
  const char *end = d_memchr (string, '\0', maxlen);
  return end ? (size_t) (end - string) : maxlen;
}



static int skip_atoi(const char **s)
{
	int i=0;

	while (isdigit(**s))
		i = i*10 + *((*s)++) - '0';
	return i;
}

/* Forward decl. needed for IP address printing stuff... */
int sprintf(char * buf, const char *fmt, ...);

/*
 * 
 */
int vsprintf(char *buf, const char *fmt, va_list args)
{
#if 0
	int len;
	unsigned long num;
	int i, base;
	char * str;
	const char *s;
	int flags;		/* flags to number() */
	char field_width;	/* width of output field */
	char precision;		/* min. # of digits for integers; max
				   number of chars for from string */

	
	for (str=buf ; *fmt ; ++fmt) {
		if (*fmt != '%') {
			*str++ = *fmt;
			continue;
		}
		/* get field width */
		field_width = -1;


		/* get the precision */
		precision = -1;

		/* default base */
		base = 10;

		switch (*fmt) {
		case 'c':
				while (--field_width > 0)
					*str++ = ' ';
			*str++ = (unsigned char) va_arg(args, int);
			while (--field_width > 0)
				*str++ = ' ';
			continue;

		case 's':
			s = va_arg(args, char *);
			if (!s)
				s = "<NULL>";

			len = strnlen(s, precision);
        	while (len < field_width--)
					*str++ = ' ';

			for (i = 0; i < len; ++i)
				*str++ = *s++;

			continue;

		case '%':
			*str++ = '%';
			continue;
		case 'X':
			flags |= LARGE;
		case 'x':
			base = 16;
			break;

		case 'd':
		case 'i':
			flags |= SIGN;
		case 'u':
			break;

		default:
			*str++ = '%';
			if (*fmt)
				*str++ = *fmt;
			else
				--fmt;
			continue;
		}

	
	    {
			num = va_arg(args, unsigned int);
			if (flags & SIGN)
				num = (signed int) num;
		}

		str = number(str, num, base, precision, flags);
	}
	*str = '\0';
	return str-buf;
	#endif
}

char d_sprintf(char * buf, const char *fmt, ...)
{
	va_list args;
	char i;

	va_start(args, fmt);
	i=vsprintf(buf,fmt,args);
	va_end(args);
	return i;
}

