#pragma once

//nrf libraries
#include "led_softblink.h"
#include "app_error.h"

//project libraries
#include "log.h"
#include "UartLogger.h"



struct led {
	void(*advertising_fast)(void);
	void(*advertising_slow)(void);
	void(*advertising_idle)(void);
	void(*ble_connected)(void);
	void(*hids_write)(void);
	void(*init)(void);			
};
extern const struct led Led;
