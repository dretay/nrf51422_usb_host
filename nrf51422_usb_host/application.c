#include "application.h"

static void init(void)
{	
	UartLogger.init();
	Usb.init();
	SoftBlink.init();
	Button.init();	
}

const struct myapplication MyApplication = { 
	.init = init,		
};

