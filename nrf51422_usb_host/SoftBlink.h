#pragma once

//nrf libraries
#include "led_softblink.h"
#include "app_error.h"

//project libraries
#include "types_shortcuts.h"

struct softblink {
	void(*init)(void);			
};

extern const struct softblink SoftBlink;