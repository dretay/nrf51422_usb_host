#pragma once

//stl libraries
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

//nrf libraries
#include "app_error.h"
#include <app_uart.h>
#include <nrf_uart.h>
#include <app_scheduler.h>

//project libraries
#include "types_shortcuts.h"

#define UART_TX_PIN 9
#define UART_RX_PIN 11
#define UART_TX_BUF_SIZE                256
#define UART_RX_BUF_SIZE                256



struct uartlogger {
	void(*init)(void);			
	void(*log_log)(bool append, bool terminate, int level, const char* file, const char* function, int line, const char* fmt, ...);
};

extern const struct uartlogger UartLogger;