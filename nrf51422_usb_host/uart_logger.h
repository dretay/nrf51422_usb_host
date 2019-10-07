#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <app_uart.h>

void log_log(bool append, bool terminate, int level, const char* file, const char* function, int line, const char* fmt, ...);