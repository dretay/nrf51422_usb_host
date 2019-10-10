#pragma once



//project libraries
#include "UartLogger.h"
#include "Button.h"
#include "SoftBlink.h"
#include "Usb.h"

struct myapplication {
	void(*init)(void);			
};

extern const struct myapplication MyApplication;