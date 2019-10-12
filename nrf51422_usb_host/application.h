#pragma once


//project libraries
#include "UartLogger.h"
#include "Button.h"
#include "SoftBlink.h"
#include "Usb.h"
#include "log.h"
#include "PlatformConfig.h"
#include "UsbDescriptorParser.h"


struct myapplication {
	void(*init)(USBDevice **usb_device);			
};

extern const struct myapplication MyApplication;