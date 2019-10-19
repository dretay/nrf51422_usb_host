#pragma once

//nrf libraries
#include "app_button.h"
#include "app_timer.h"
#include "nrf_drv_gpiote.h"

//project libraries
#include "log.h"
#include "UartLogger.h"

#define BUTTON_LONG_PUSH (2)

#define MS_TO_TICK(MS) (m_app_ticks_per_100ms * (MS / 100))
#define LONG_PUSH_TIMEOUT_MS 1000

APP_TIMER_DEF(button_timer_id);

#define BUTTON_LIST_MAX 1
typedef struct
{
	u8 pin;
	void(*push)(void);
	void(*long_push)(void);
	void(*release)(void);
	void(*startup)(void);
} button_config;

struct button {
	bool(*add_button)(u8 pin, void* push_callback, void* long_push_callback, void* release_callback, void* startup_callback);
	void(*start)(void);			
};
extern const struct button Button;