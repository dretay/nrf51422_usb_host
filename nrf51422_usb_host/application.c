#include "application.h"

static void init(USBDevice **usb_device)
{	
	
	PlatformConfig platform_config;
	platform_config.log_log = UartLogger.log_log;

	UartLogger.init();
	*usb_device = Usb.init();
	SoftBlink.init();
	Button.init();	

	
}

const struct myapplication MyApplication = { 
	.init = init,		
};

