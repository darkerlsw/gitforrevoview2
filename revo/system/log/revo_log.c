#include "revo_os.h"
#include "revo_log_uart.h"
#include "revo_log.h"

void itof(char **buf, int i)
{
	revo_char *s;
#define LEN	20
	revo_s32 rem, j;
	static char rev[LEN+1];

	rev[LEN] = 0;
	s = &rev[LEN];
	for (j= 0 ; j < 4 ; j++)
	{
		rem = i % 10;
			*--s = rem + '0';
		i /= 10;
	}
	while (*s)
	{
		(*buf)[0] = *s++;
		++(*buf);
	}
}

static void itoa(char **buf, int i, int base)
{
	char *s;
#define LEN	20
	int rem;
	static char rev[LEN+1];

	rev[LEN] = 0;
	if (i == 0)
	{
		(*buf)[0] = '0';
		++(*buf);
		return;
	}
	
	s = &rev[LEN];
	while (i)
	{
		rem = i % base;
		if (rem < 10)
			*--s = rem + '0';
		else if (base == 16)
			*--s = "abcdef"[rem - 10];
		i /= base;
	}
	
	while (*s)
	{
		(*buf)[0] = *s++;
		++(*buf);
	}
}

static void itoa_zp(char **buf, int i, int base, int bit)
{
	revo_char *s;
#define LEN	20
	revo_s32 rem, len;
	static revo_char rev[LEN+1];

	if(bit <= 0)
		return;

	rev[LEN] = 0;
	len = 0;
	if (i == 0)
	{
		while(bit--)
		{
			(*buf)[0] = '0';
			++(*buf);
		}
		
		return;
	}
	s = &rev[LEN];
	
	while (i)
	{
		rem = i % base;
		if (rem < 10)
			*--s = rem + '0';
		else if (base == 16)
			*--s = "abcdef"[rem - 10];
		len++;
		i /= base;
	}

	while(len < bit)
	{
		(*buf)[0] = '0';
		++(*buf);
		len++;
	}
	
	while (*s)
	{
		(*buf)[0] = *s++;
		++(*buf);
	}
}

static void addchar(char **buf, char a)
{
	(*buf)[0] = a;
	++(*buf);
}

static void timetoa(char **buf)
{
	revo_u32 sec,min,hour;

	sec = revo_uptime_ms() / 1000;
	min = sec / 60;
	sec = sec % 60;
	hour = min /60;
	min = min % 60;
	hour = hour % 24;

	itoa_zp(buf, hour, 10, 2);
	addchar(buf, ':');
	itoa_zp (buf, min, 10, 2);
	addchar(buf, ':');
	itoa_zp (buf, sec, 10, 2);
	addchar(buf, ' ');
	addchar(buf, ' ');
}

revo_u32 revo_log_init(void)
{
	revo_u32 ret;
	static revo_bool is_init = REVO_FALSE;

	if(is_init)
		return;
	
	ret = revo_log_uart_open();
	if(ret == REVO_FAIL)
		return ret;

	is_init = REVO_TRUE;
	revo_log(NULL, "\r\n");
	revo_log(NULL, "###### revo sdk start ######");
}

void revo_log(const revo_char *tag, const char *fmt, ...)
{
	va_list ap;
	double dval;
	revo_s32 ival;
	revo_char *p, *sval;
	revo_char *bp, cval;
	revo_s32 fract;
	revo_u16 len;
	revo_char print_buf[512] = {0};
	revo_u32 ret;

	bp= print_buf;
	timetoa(&bp);

	if(tag != NULL)
	{
		addchar(&bp, '[');
		for (; *tag; tag++)
			*bp++ = *tag;
		addchar(&bp, ']');
	}

	va_start(ap, fmt);

	for (p= fmt; *p; p++)
	{
		if (*p != '%')
		{
			*bp ++= *p;
			continue;
		}

		switch (*++p){
			case 'd':
				ival = va_arg(ap, int);
				if (ival < 0){
					*bp ++= '-';
					ival = -ival;
				}
				itoa(&bp, ival, 10);
				break;

			case 'o':
				ival = va_arg(ap, int);
				if (ival < 0){
					*bp ++= '-';
					ival = -ival;
				}
				*bp ++= '0';
				itoa(&bp, ival, 8);
				break;

			case 'x':
				ival = va_arg(ap, int);
				if (ival < 0){
					*bp++= '-';
					ival = -ival;
				}
				*bp ++= '0';
				*bp ++= 'x';
				itoa(&bp, ival, 16);
				break;

			case 'c':
				cval = va_arg(ap, int);
				*bp ++= cval;
				break;

			case 'f':
				dval = va_arg(ap, double);
				if (dval < 0){
					*bp ++= '-';
					dval = -dval;
				}
				if (dval >= 1.0)
					itoa(&bp, (int)dval, 10);
				else
					*bp ++= '0';
				*bp ++= '.';
				fract = (int)((dval- (double)(int)dval)*(double)(10000));
				itof(&bp, fract);
				break;

			case 's':
				for (sval = va_arg(ap, char *) ; *sval ; sval++ )
					*bp++= *sval;
				break;
		}
	}

	if(!(((*(bp - 2) == '\r') && (*(bp - 1) == '\n')) || \
		((*(bp - 2) == '\n') && (*(bp - 1) == '\r')) || \
			(*(bp-1) == '\n')))
	{
		*bp++='\r';
		*bp++='\n';
	}
	
	*bp = 0;
	len = (unsigned short)(bp - print_buf);

	for (bp = print_buf; *bp; bp++)
	{
		ret = revo_log_uart_write(bp, 1);
		if(ret == 0)
			bp--;
	}

	va_end(ap);
}

