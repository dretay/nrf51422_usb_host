#pragma once


//project libraries
#include "UartLogger.h"
#include "Button.h"
#include "SoftBlink.h"
#include "Usb.h"
#include "log.h"
#include "PlatformConfig.h"
#include "UsbDescriptorParser.h"

//nrf51 libraries
#include "nordic_common.h"
#include "ble_hids.h"

struct application {
	void(*init)(USBDevice **usb_device);			
};

extern const struct application Application;