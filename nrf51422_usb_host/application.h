#pragma once

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "USBCore.h"
#include "HIDUniversal.h"
#include "nrf_drv_spi.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "uart_logger.h"
#include "app_timer.h"
#include "nrf_gpio.h"
#include "app_scheduler.h"
#include "nrf_uart.h"
#include "app_timer_appsh.h"

#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"

#define MAX_CS_PIN		GPIO_PIN_4
#define MAX_CS_PORT		GPIOA
#define MAX_RST_PIN		GPIO_PIN_3
#define MAX_RST_PORT	GPIOA

#define START_SPI_TRANSACTION HAL_GPIO_WritePin(MAX_CS_PORT, MAX_CS_PIN, GPIO_PIN_RESET);
#define END_SPI_TRANSACTION HAL_GPIO_WritePin(MAX_CS_PORT, MAX_CS_PIN, GPIO_PIN_SET);

// usb poll interval  (20ms)
#define USB_HOST_POLL_INTERVAL APP_TIMER_TICKS(20, 0)
APP_TIMER_DEF(m_usb_host_timer_id);


struct myapplication {
	void(*init)(void);			
};

extern const struct myapplication MyApplication;