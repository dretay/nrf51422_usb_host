#include "uart_logger.h"

static void uart_putstring(const uint8_t * str)
{
	uint32_t err_code;
    
	uint8_t len = strlen((char *) str);
	for (uint8_t i = 0; i < len; i++)
	{
		err_code = app_uart_put(str[i]);
		APP_ERROR_CHECK(err_code);
	}
    
}
static void myvprint(const char *fmt, va_list argp)
{
	char string[200];
	if (0 < vsprintf(string, fmt, argp)) // build string
		{
			uart_putstring(string);          // send message via UART
		}
}

static const char* level_names[] = {
	"TRACE",
	"DEBUG",
	"INFO",
	"WARN",
	"ERROR",
	"FATAL"
};
static const char* level_colors[] = {
	"\x1b[94m",
	"\x1b[36m",
	"\x1b[32m",
	"\x1b[33m",
	"\x1b[31m",
	"\x1b[35m"
};
void log_log(bool append, bool terminate, int level, const char* file, const char* function, int line, const char* fmt, ...)
{
	
	va_list args;
	char buf[100] = { 0 };

	if (!append) {

		snprintf(buf,
			100,
			"%s%s %s%-5s\x1b[0m \x1b[90m%s:%s:%d:\x1b[0m ",
			buf,
			"\r\n",
			level_colors[level],
			level_names[level],
			file,
			function,
			line);
		uart_putstring(buf);
	}

	va_start(args, fmt);
	myvprint(fmt, args);	
	va_end(args);
	if (terminate) {		

		snprintf(buf,
			2,
			"\r\n",
			buf);
		uart_putstring(buf);
	}
}