#pragma once

//nrf libraries
#include "nrf_drv_spi.h"
#include "nrf_delay.h"
#include "app_scheduler.h"
#include "app_error.h"
#include "nrf_gpio.h"
#include "app_timer.h"


//project libraries
#include "USBCore.h"
#include "HIDUniversal.h"
#include "UartLogger.h"
#include "log.h"

#define SPI_SS_PIN 15
#define SPI_MOSI_PIN 16
#define SPI_MISO_PIN 17
#define SPI_SCK_PIN 18
#define SPI_RST_PIN 19

#define MAX_CS_PIN		GPIO_PIN_4
#define MAX_CS_PORT		GPIOA
#define MAX_RST_PIN		GPIO_PIN_3
#define MAX_RST_PORT	GPIOA

#define START_SPI_TRANSACTION HAL_GPIO_WritePin(MAX_CS_PORT, MAX_CS_PIN, GPIO_PIN_RESET);
#define END_SPI_TRANSACTION HAL_GPIO_WritePin(MAX_CS_PORT, MAX_CS_PIN, GPIO_PIN_SET);

// usb poll interval  (20ms)
#define USB_HOST_POLL_INTERVAL APP_TIMER_TICKS(20, 0)
APP_TIMER_DEF(m_usb_host_timer_id);


struct usb {
	void(*init)(void);			
};

extern const struct usb Usb;